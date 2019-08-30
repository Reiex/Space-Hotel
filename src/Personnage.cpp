#include "Personnage.h"


// FONCTIONS MEMBRES DE LA CLASSE NOEUD


Noeud::Noeud()
{
	m_type = Noeud::Type::Normal;
}


void Noeud::connecter(Noeud* noeud)
{
	bool presentThis(false), presentNoeud(false);

	for (int i(0); i < m_voisins.size(); i++)
	{
		if (m_voisins[i] == noeud)
		{
			presentNoeud = true;
			break;
		}
	}
	for (int i(0); i < noeud->m_voisins.size(); i++)
	{
		if (noeud->m_voisins[i] == this)
		{
			presentThis = true;
			break;
		}
	}

	if (!presentThis)
	{
		noeud->m_voisins.push_back(this);
	}
	if (!presentNoeud)
	{
		m_voisins.push_back(noeud);
	}
}


void Noeud::trouverTrajectoire(Noeud* noeud, std::queue<Noeud*>& trajectoire)
{
	class Chemin
	{
		public:

			Chemin(Noeud* noeud, Chemin* chemin)
			{
				m_noeud = noeud;
				m_cheminParent = chemin;
			}

			Noeud* m_noeud;
			Chemin* m_cheminParent;
	};

	std::queue<Chemin*> cheminsDisponnibles;
	Chemin* cheminEnCours(new Chemin(this, 0));
	std::vector<Noeud*> noeudsMarques;
	noeudsMarques.push_back(this);

	while (cheminEnCours->m_noeud != noeud)
	{
		for (int i(0); i < cheminEnCours->m_noeud->m_voisins.size(); i++)
		{
			bool marque(false);

			for (int j(0); j < noeudsMarques.size(); j++)
			{
				if (noeudsMarques[j] == cheminEnCours->m_noeud->m_voisins[i])
				{
					marque = true;
					break;
				}
			}

			if (!marque)
			{
				cheminsDisponnibles.push(new Chemin(cheminEnCours->m_noeud->m_voisins[i], cheminEnCours));
				noeudsMarques.push_back(cheminEnCours->m_noeud->m_voisins[i]);
			}
		}

		if (cheminsDisponnibles.size() == 0)
		{
			break;
		}

		cheminEnCours = cheminsDisponnibles.front();
		cheminsDisponnibles.pop();
	}

	std::vector<Noeud*> cheminInverse;
	cheminInverse.push_back(cheminEnCours->m_noeud);

	while (cheminEnCours->m_cheminParent != 0)
	{
		cheminEnCours = cheminEnCours->m_cheminParent;
		cheminInverse.push_back(cheminEnCours->m_noeud);
	}

	for (int i(cheminInverse.size() - 1); i >= 0; i--)
	{
		trajectoire.push(cheminInverse[i]);
	}
}


void Noeud::setOccupe(bool occupe)
{
	m_occupe = occupe;
}


bool Noeud::estLibre() const
{
	if (m_type == Noeud::Type::Normal)
	{
		return true;
	}
	else if (m_type == Noeud::Type::Connexion)
	{
		return false;
	}

	return !m_occupe;
}


int Noeud::getType() const
{
	return m_type;
}


void Noeud::setType(int type)
{
	m_type = type;
}


Noeud::~Noeud()
{
	for (int i(0); i < m_voisins.size(); i++)
	{
		int k(-1);

		for (int j(0); j < m_voisins[i]->m_voisins.size(); j++)
		{
			if (m_voisins[i]->m_voisins[j] == this)
			{
				k = j;
			}
		}

		m_voisins[i]->m_voisins.erase(m_voisins[i]->m_voisins.begin() + k);
	}
}


// FONCTIONS MEMBRES DE LA CLASSE PERSONNAGE


Personnage::Personnage(Noeud* noeud) : Conteneur(1)
{
	m_arrete[0] = noeud;
	m_arrete[1] = noeud;

	m_avancement = 1;

	m_w = 64;
	m_h = 64;
	m_direction = Personnage::Direction::Droite;

	m_machineCible = 0;
	m_emplacementCible = 0;
	m_action = Personnage::Ballade;
}


Noeud* Personnage::getDepart()
{
	return m_arrete[0];
}


Noeud* Personnage::getArrivee()
{
	return m_arrete[1];
}


void Personnage::afficher(sf::RenderWindow& window, Loader& loader) const
{
	// Afficher le personnage

	sf::Sprite sprite(*(loader.obtenirTexture("images/personnages/normal.png")));
	sf::Vector2f scale(1, 1), position(m_x - 32, m_y - 32);

	if (m_direction == Personnage::Direction::Gauche)
	{
		scale.x = -1;
		position.x += 64;
	}

	sprite.setScale(scale);
	sprite.setPosition(position);

	window.draw(sprite);

	// Afficher sa ressource s'il en a une

	if (m_emplacements[0].estOccupe())
	{
		sprite = sf::Sprite(*(loader.obtenirTexture(Ressource::cheminsTypes[getTypeRessource(0)])));

		sf::Shader* shader(loader.obtenirShader("shaders/masqueRessources.frag"));
		shader->setUniform("textureCourrante", sf::Shader::CurrentTexture);
		shader->setUniform("textureMasque", *loader.obtenirTexture("images/personnages/masque normal.png"));

		position.x += scale.x*40;
		position.y += 28;
		scale.x *= 0.23;
		scale.y *= 0.23;
		sprite.setScale(scale);
		sprite.setPosition(position);

		window.draw(sprite, shader);
	}
}


void Personnage::setDirection(int direction)
{
	m_direction = direction;
}


void Personnage::avancer(float dt, float distance)
{
	m_avancement += dt / (distance/100 + dt);

	if (m_avancement >= 1)
	{
		m_avancement = 1;
		m_arrete[0] = m_arrete[1];

		if (m_trajectoire.size() > 0)
		{
			m_avancement = 0;

			m_arrete[1] = m_trajectoire.front();
			m_trajectoire.pop();
		}
	}
}


void Personnage::ajusterTrajectoire(Noeud* noeud)
{
	while (m_trajectoire.size() > 0)
	{
		m_trajectoire.pop();
	}

	m_arrete[1]->trouverTrajectoire(noeud, m_trajectoire);
}


void Personnage::annulerTrajectoire()
{
	while (!m_trajectoire.empty())
	{
		m_trajectoire.pop();
	}
}


bool Personnage::traverseNoeud(Noeud* noeud)
{
	bool traverse(false);

	for (int i(0); i < m_trajectoire.size(); i++)
	{
		if (m_trajectoire.front() == noeud)
		{
			traverse = true;
		}

		m_trajectoire.push(m_trajectoire.front());
		m_trajectoire.pop();
	}

	return traverse;
}


Machine* Personnage::getMachineCible() const
{
	return m_machineCible;
}


void Personnage::setMachineCible(Machine* machine)
{
	m_machineCible = machine;
}


int Personnage::getEmplacementCible() const
{
	return m_emplacementCible;
}


void Personnage::setEmplacementCible(int emplacement)
{
	m_emplacementCible = emplacement;
}


Personnage::Action Personnage::getAction() const
{
	return m_action;
}


void Personnage::setAction(Personnage::Action action)
{
	m_action = action;
}


bool Personnage::estOccupe()
{
	return !(m_avancement >= 1 && m_trajectoire.size() == 0);
}


float Personnage::getAvancement() const
{
	return m_avancement;
}


// FONCTIONS QUI NE SONT PAS MEMBRES D'UNE CLASSE


void interconnecterNoeuds(std::vector<Noeud*>* listeNoeuds)
{
	for (int i(0); i < listeNoeuds->size(); i++)
	{
		for (int j(0); j < listeNoeuds->size(); j++)
		{
			if (i != j)
			{
				(*listeNoeuds)[i]->connecter((*listeNoeuds)[j]);
			}
		}
	}
}
