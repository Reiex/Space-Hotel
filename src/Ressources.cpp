#include "Ressources.h"


// FONCTIONS MEMBRES DE LA CLASSE CONTENEUR


Conteneur::Conteneur(int nbEmplacements)
{
	for (int i(0); i < nbEmplacements; i++)
	{
		m_emplacements.push_back(EmplacementRessource(true));
	}
}


bool Conteneur::ressourceNecessaire(Ressource* ressource) const
{
	for (int i(0); i < m_emplacements.size(); i++)
	{
		if (!m_emplacements[i].estOccupe() && m_emplacements[i].estAutorisee(ressource) && m_emplacements[i].ressourceNecessaire())
		{
			return true;
		}
	}

	return false;
}


bool Conteneur::ressourceDeposable(Ressource* ressource) const
{
	for (int i(0); i < m_emplacements.size(); i++)
	{
		if (!m_emplacements[i].estOccupe() && m_emplacements[i].estAutorisee(ressource))
		{
			return true;
		}
	}

	return false;
}


void Conteneur::deposerRessource(Ressource* ressource)
{
	for (int i(0); i < m_emplacements.size(); i++)
	{
		if (!m_emplacements[i].estOccupe() && m_emplacements[i].estAutorisee(ressource))
		{
			m_emplacements[i].setRessource(ressource);
			ressource->setConteneur(this);
			break;
		}
	}
}


bool Conteneur::ressourcePresente(int i) const
{
	return (m_emplacements[i].getRessource() != 0);
}


int Conteneur::getTypeRessource(int i) const
{
	return m_emplacements[i].getRessource()->getType();
}


Ressource* Conteneur::getRessource(int i)
{
	return m_emplacements[i].getRessource();
}


void Conteneur::retirerRessource(Ressource* ressource)
{
	for (int i(0); i < m_emplacements.size(); i++)
	{
		if (m_emplacements[i].getRessource() == ressource)
		{
			m_emplacements[i].setRessource(0);
		}
	}
}


Conteneur::~Conteneur()
{
	for (int i(0); i < m_emplacements.size(); i++)
	{
		if (m_emplacements[i].getRessource() != 0)
		{
			m_emplacements[i].getRessource()->setConteneur(0);
		}
	}
}


// FONCTIONS MEMBRES DE LA CLASSE EMPLACEMENT_RESSOURCE


EmplacementRessource::EmplacementRessource(bool entree)
{
	m_entree = entree;
	m_ressourceNecessaire = false;
	m_ressource = 0;

	for (int i(0); i < Ressource::nbTypes; i++)
	{
		m_autorisees.push_back(i);
	}
}


void EmplacementRessource::interdireRessource(int type)
{
	for (int i(0); i < m_autorisees.size(); i++)
	{
		if (m_autorisees[i] == type)
		{
			m_autorisees.erase(m_autorisees.begin() + i);
			i--;
		}
	}
}


void EmplacementRessource::autoriserRessource(int type)
{
	bool estAutorisee(false);

	for (int i(0); i < m_autorisees.size(); i++)
	{
		estAutorisee = estAutorisee || (m_autorisees[i] == type);
	}

	if (!estAutorisee)
	{
		m_autorisees.push_back(type);
	}
}


bool EmplacementRessource::estAutorisee(Ressource* ressource) const
{
	int type(ressource->getType());

	for (int i(0); i < m_autorisees.size(); i++)
	{
		if (type == m_autorisees[i])
		{
			return true;
		}
	}

	return false;
}


void EmplacementRessource::setEntree(bool entree)
{
	m_entree = entree;
}


void EmplacementRessource::setRessourceNecessaire(bool necessaire)
{
	m_ressourceNecessaire = necessaire;
}


bool EmplacementRessource::estOccupe() const
{
	return (m_ressource != 0) || (!m_entree);
}


bool EmplacementRessource::ressourceNecessaire() const
{
	return m_ressourceNecessaire;
}


void EmplacementRessource::setRessource(Ressource* ressource)
{
	m_ressource = ressource;
}


Ressource* EmplacementRessource::getRessource() const
{
	return m_ressource;
}


// FONCTIONS MEMBRES DE LA CLASSE RESSOURCE


int const Ressource::nbTypes(1);
std::string const Ressource::cheminsTypes[Ressource::nbTypes] = {
		"images/interface/logo eau.png"
	};


Ressource::Ressource(int type)
{
	m_type = type;
	m_etat = 100;
	m_conteneur = 0;
	m_noeudProche = 0;
}


int Ressource::getType() const
{
	return m_type;
}


Conteneur* Ressource::getConteneur() const
{
	return m_conteneur;
}


void Ressource::setConteneur(Conteneur* conteneur)
{
	m_conteneur = conteneur;
}


Noeud* Ressource::getNoeudProche() const
{
	return m_noeudProche;
}


void Ressource::setNoeudProche(Noeud* noeud)
{
	m_noeudProche = noeud;
}


float Ressource::getEtat() const
{
	return m_etat;
}


void Ressource::setEtat(float etat)
{
	m_etat = etat;
}


Ressource::~Ressource()
{
	m_conteneur->retirerRessource(this);
}


// FONCTIONS MEMBRES DE LA CLASSE MACHINE


Entite const Machine::SurfaceDessinDetails(15, 15, 474, 230);


Machine::Machine(int nbEntrees, int nbSorties) : Conteneur(nbEntrees + nbSorties)
{
	for (int i(nbEntrees); i < nbEntrees + nbSorties; i++)
	{
		m_emplacements[i].setEntree(false);
	}

	m_energieConso = 0;
	m_texture = 0;
}


void Machine::faireFonctionner(float dt)
{

}


float Machine::getEnergieConso() const
{
	return m_energieConso;
}


float Machine::getChaleurDissipee() const
{
	return m_chaleurDissipee;
}


void Machine::effectuerRotation(Loader& loader)
{

}


sf::Texture* Machine::getTexture()
{
	return m_texture;
}


void Machine::setNoeudProche(Noeud* noeud)
{
	m_noeudProche = noeud;
}


Noeud* Machine::getNoeudProche() const
{
	return m_noeudProche;
}


Machine::~Machine()
{
	// TODO: Eparpiller les ressources plutôt que de les mettres sur la machine

	for (int i(0); i < m_emplacements.size(); i++)
	{
		Ressource* ressource(m_emplacements[i].getRessource());
		if (ressource != 0)
		{
			ressource->setNoeudProche(m_noeudProche);
			ressource->setCoord(m_x, m_y, ressource->getW(), ressource->getH());
		}
	}
}


// FONCTIONS MEMBRES DE LA CLASSE TERMINAL_RADIATEUR


TerminalRadiateur::TerminalRadiateur(Loader& loader) : Machine(2, 0)
{
	m_emplacements[0].setRessourceNecessaire(true);
	m_emplacements[1].setRessourceNecessaire(true);

	for (int i(0); i < Ressource::nbTypes; i++)
	{
		if (i != Ressource::Eau)
		{
			m_emplacements[0].interdireRessource(i);
			m_emplacements[1].interdireRessource(i);
		}
	}

	m_x = 65;
	m_y = 57;
	m_w = 44;
	m_h = 68;

	m_texture = loader.obtenirTexture("images/salle de refroidissement/verticale/terminal radiateur.png");

	m_energieConso = 2;
	m_chaleurDissipee = 0;
}


void TerminalRadiateur::afficherDetails(sf::RenderWindow& window, Loader& loader) const
{
	int x(SurfaceDessinDetails.getX()), y(SurfaceDessinDetails.getY()), w(SurfaceDessinDetails.getW()), h(SurfaceDessinDetails.getH());

	sf::Texture* texture;
	sf::Sprite sprite;
	sf::Text texte;
	texte.setFont(*(loader.getFont()));
	texte.setFillColor(sf::Color(5, 15, 6));
	texte.setCharacterSize(25);

	/*
		Consommation électrique: [] X

		Alimentation du radiateur:
		
			[] [] ===> [Thermometre] [Thermometre]

		Chaleur dissipée: [] X
	
	*/

	// Consommation électrique
	
	texte.setString(sf::String("Consommation électrique:"));
	texte.setPosition(x, y + 5);
	window.draw(texte);

	int wTexte(texte.getGlobalBounds().width);
	texture = loader.obtenirTexture("images/interface/logo electricite.png");
	sprite.setTexture(*texture);
	sprite.setPosition(x + wTexte + 10, y + 3);
	sprite.setScale(0.3, 0.3);
	window.draw(sprite);

	texte.setString(sf::String(float_to_string(m_energieConso)));
	texte.setPosition(x + wTexte + 50, y);
	texte.setCharacterSize(30);
	window.draw(texte);

	texte.setCharacterSize(25);

	// Alimentation du radiateur

	texte.setString(sf::String("Alimentation du radiateur:"));
	texte.setPosition(x, y + h/2 - 50);
	window.draw(texte);

	int nbEau(m_emplacements[0].estOccupe() + m_emplacements[1].estOccupe());
	if (nbEau >= 1)
	{
		texture = loader.obtenirTexture("images/interface/logo eau.png");
	}
	else
	{
		texture = loader.obtenirTexture("images/interface/logo ressource manquante.png");
	}
	sprite.setTexture(*texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
	sprite.setPosition(x + (w - 200)/2 , y + h/2 + 20);
	sprite.setScale(0.3, 0.3);
	window.draw(sprite);

	if (nbEau >= 2)
	{
		texture = loader.obtenirTexture("images/interface/logo eau.png");
	}
	else
	{
		texture = loader.obtenirTexture("images/interface/logo ressource manquante.png");
	}
	sprite.setTexture(*texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
	sprite.setPosition(x + (w - 200)/2 + 40, y + h/2 + 20);
	sprite.setScale(0.3, 0.3);
	window.draw(sprite);

	texture = loader.obtenirTexture("images/interface/detail machine/progression.png");
	sprite.setTexture(*texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 84, 30));
	sprite.setPosition(x + (w - 200)/2 + 78, y + h/2 + 20);
	sprite.setScale(1, 1);
	window.draw(sprite);

	sprite.setTextureRect(sf::IntRect(0, 30, 37.5*nbEau, 22));
	sprite.setPosition(x + (w - 200)/2 + 81, y + h/2 + 24);
	window.draw(sprite);

	texture = loader.obtenirTexture("images/interface/logo temperature.png");
	sprite.setTexture(*texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
	sprite.setPosition(x + (w - 200)/2 + 170, y + h/2 + 20);
	sprite.setScale(0.3, 0.3);
	window.draw(sprite);

	// Chaleur dissipée

	texte.setString(sf::String("Chaleur dissipée:"));
	texte.setPosition(x, y + h - 35);
	window.draw(texte);

	wTexte = texte.getGlobalBounds().width;
	texture = loader.obtenirTexture("images/interface/logo temperature.png");
	sprite.setTexture(*texture);
	sprite.setPosition(x + wTexte + 10, y + h - 37);
	window.draw(sprite);

	texte.setString(sf::String(float_to_string(m_chaleurDissipee)));
	texte.setPosition(x + wTexte + 50, y + h - 40);
	texte.setCharacterSize(30);
	window.draw(texte);
}


void TerminalRadiateur::faireFonctionner(float dt)
{
	m_chaleurDissipee = 0;

	if (ressourcePresente(0) && getTypeRessource(0) == Ressource::Eau)
	{
		m_chaleurDissipee += 20;
	}

	if (ressourcePresente(1) && getTypeRessource(1) == Ressource::Eau)
	{
		m_chaleurDissipee += 20;
	}
}


void TerminalRadiateur::effectuerRotation(Loader& loader)
{
	if (m_texture == loader.obtenirTexture("images/salle de refroidissement/verticale/terminal radiateur.png"))
	{
		m_x = 57;
		m_y = 69;
		m_w = 68;
		m_h = 45;

		m_texture = loader.obtenirTexture("images/salle de refroidissement/horizontale/terminal radiateur.png");
	}
	else
	{
		m_x = 65;
		m_y = 57;
		m_w = 44;
		m_h = 68;

		m_texture = loader.obtenirTexture("images/salle de refroidissement/verticale/terminal radiateur.png");
	}
}
