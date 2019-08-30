#include "Salle.h"


// FONCTIONS MEMBRES DE LA CLASSE SALLE

int const Salle::NombreDeFlags(4);
float const Salle::DistanceFavorableConnexion(40);


Salle::Salle()
{
	m_root = false;
	m_impasse = false;

	m_energieConso = 1;
	m_energieProduite = 0;
	m_energieSolaireProduite = 0;
	m_capaciteAccu = 0;
	m_energieAccu = 0;
	m_chaleurDissipee = 0.5;
}


void Salle::setEnergieConso(float energie)
{
	m_energieConso = energie;
}


void Salle::setEnergieProduite(float energie)
{
	m_energieProduite = energie;
}


void Salle::setEnergieSolaireProduite(float energie)
{
	m_energieSolaireProduite = energie;
}


void Salle::setCapaciteAccu(float capacite)
{
	m_capaciteAccu = capacite;
}


void Salle::setEnergieAccu(float energie)
{
	m_energieAccu = energie;
}


void Salle::setChaleurDissipee(float chaleur)
{
	m_chaleurDissipee = chaleur;
}


void Salle::setPanneElectrique(bool panne)
{
	m_panneElectrique = panne;
}


void Salle::faireFonctionner(float dt)
{
	for (int i(0); i < m_machines.size(); i++)
	{
		m_machines[i]->faireFonctionner(dt, m_panneElectrique);
	}
}


bool Salle::appartenanceNoeud(Noeud* noeud) const
{
	for (int i(0); i < m_noeuds.size(); i++)
	{
		if (m_noeuds[i] == noeud)
		{
			return true;
		}
	}

	return false;
}


void Salle::deplacerPersonnage(Personnage* personnage, float dt) const
{
	// Déterminer les coordonnées absolues des points d'arrivée et de départ

	int pointDepart[2] = { 0, 0 }, pointArrivee[2] = { 0, 0 };
	Noeud *depart(personnage->getDepart()), *arrivee(personnage->getArrivee());

	pointDepart[0] = depart->getX() + m_x;
	pointDepart[1] = depart->getY() + m_y;

	if (appartenanceNoeud(arrivee))
	{
		pointArrivee[0] = arrivee->getX() + m_x;
		pointArrivee[1] = arrivee->getY() + m_y;
	}
	else
	{
		for (int i(0); i < m_voisines.size(); i++)
		{
			if (m_voisines[i]->appartenanceNoeud(arrivee))
			{
				pointArrivee[0] = arrivee->getX() + m_voisines[i]->m_x;
				pointArrivee[1] = arrivee->getY() + m_voisines[i]->m_y;
				break;
			}
		}
	}

	float distance(sqrt(pow(pointDepart[0] - pointArrivee[0], 2) + pow(pointDepart[1] - pointArrivee[1], 2)));

	// Faire avancer le personnage

	float t(personnage->getAvancement());
	personnage->setCoord(pointArrivee[0] * t + pointDepart[0] * (1 - t), pointArrivee[1] * t + pointDepart[1] * (1 - t));
	personnage->avancer(dt, distance);

	// Tourner le personnage

	if (pointArrivee[0] - pointDepart[0] >= 0)
	{
		personnage->setDirection(Personnage::Direction::Droite);
	}
	else
	{
		personnage->setDirection(Personnage::Direction::Gauche);
	}

	// Donner une effet de flottement au personnage

	if (pointArrivee[0] - pointDepart[0] != 0)
	{
		personnage->setCoord(personnage->getX(), personnage->getY() + 2 * sin(t * 2.0f * 3.1415926f * (distance / 100)));
	}
	else if (pointArrivee[1] - pointDepart[1] != 0)
	{
		personnage->setCoord(personnage->getX() + 2 * sin(t * 2.0f * 3.1415926f * (distance / 100)), personnage->getY());
	}

	// Déplacer la ressource du personnage avec lui

	if (personnage->ressourcePresente(0))
	{
		Ressource* ressource(personnage->getRessource(0));
		ressource->setCoord(personnage->getX(), personnage->getY(), ressource->getW(), ressource->getH());
	}
}


bool Salle::estTraversee(Personnage* personnage) const
{
	for (int i(0); i < m_noeuds.size(); i++)
	{
		if (personnage->traverseNoeud(m_noeuds[i]))
		{
			return true;
		}
	}
	
	return false;
}


bool Salle::contientPersonnage(Personnage* personnage) const
{
	for (int i(0); i < m_noeuds.size(); i++)
	{
		if (m_noeuds[i] == personnage->getDepart() || m_noeuds[i] == personnage->getArrivee())
		{
			return true;
		}
	}

	return false;
}


void Salle::effectuerRotation(Loader& loader)
{

}


bool Salle::essayerConnecterSalle(Salle& nouvelleSalle) const
{
	if (m_impasse)
	{
		return false;
	}

	// Rechercher une connexion libre suffisamment proche d'une autre

	float d_min(DistanceFavorableConnexion + 1), d(DistanceFavorableConnexion + 1);
	int connexionIn(-1), connexionOut(-1);

	int xCentreA, yCentreA, xCentreB, yCentreB;

	for (int i(0); i < m_connexions.size(); i++)
	{
		for (int j(0); j < nouvelleSalle.m_connexions.size(); j++)
		{
			if (m_connexions[i].connexionCompatible(nouvelleSalle.m_connexions[j]) && !m_connexions[i].estConnectee())
			{
				xCentreA = m_x + m_connexions[i].getX() + m_connexions[i].getW() / 2;
				yCentreA = m_y + m_connexions[i].getY() + m_connexions[i].getH() / 2;
				xCentreB = nouvelleSalle.m_x + nouvelleSalle.m_connexions[j].getX() + nouvelleSalle.m_connexions[j].getW() / 2;
				yCentreB = nouvelleSalle.m_y + nouvelleSalle.m_connexions[j].getY() + nouvelleSalle.m_connexions[j].getH() / 2;

				d = sqrt(pow(xCentreA - xCentreB, 2) + pow(yCentreA - yCentreB, 2));

				if (d < d_min)
				{
					d_min = d;
					connexionIn = i;
					connexionOut = j;
					break;
				}
			}
		}

		if (connexionIn != -1)
		{
			break;
		}
	}

	// S'il y en a une, la déplacer pour préparer la connexion

	if (d_min <= DistanceFavorableConnexion)
	{
		if (m_connexions[connexionIn].getDirection() == Connexion::Direction::Bas)
		{
			nouvelleSalle.m_x = m_x + m_connexions[connexionIn].getX() + m_connexions[connexionIn].getW() / 2 - nouvelleSalle.m_connexions[connexionOut].getX() - nouvelleSalle.m_connexions[connexionOut].getW() / 2;
			nouvelleSalle.m_y = m_y + m_h;
		}
		else if (m_connexions[connexionIn].getDirection() == Connexion::Direction::Haut)
		{
			nouvelleSalle.m_x = m_x + m_connexions[connexionIn].getX() + m_connexions[connexionIn].getW() / 2 - nouvelleSalle.m_connexions[connexionOut].getX() - nouvelleSalle.m_connexions[connexionOut].getW() / 2;
			nouvelleSalle.m_y = m_y - nouvelleSalle.m_h;
		}
		else if (m_connexions[connexionIn].getDirection() == Connexion::Direction::Droite)
		{
			nouvelleSalle.m_x = m_x + m_w;
			nouvelleSalle.m_y = m_y + m_connexions[connexionIn].getY() + m_connexions[connexionIn].getH() / 2 - nouvelleSalle.m_connexions[connexionOut].getY() - nouvelleSalle.m_connexions[connexionOut].getH() / 2;
		}
		else if (m_connexions[connexionIn].getDirection() == Connexion::Direction::Gauche)
		{
			nouvelleSalle.m_x = m_x - nouvelleSalle.m_w;
			nouvelleSalle.m_y = m_y + m_connexions[connexionIn].getY() + m_connexions[connexionIn].getH() / 2 - nouvelleSalle.m_connexions[connexionOut].getY() - nouvelleSalle.m_connexions[connexionOut].getH() / 2;
		}

		nouvelleSalle.m_connexions[connexionOut].connecter();

		return true;
	}
	else
	{
		return false;
	}
}


void Salle::connecterSalle(Salle& nouvelleSalle)
{
	if (!essayerConnecterSalle(nouvelleSalle))
	{
		return;
	}

	int xCentreA, yCentreA, xCentreB, yCentreB;

	for (int i(0); i < nouvelleSalle.m_connexions.size(); i++)
	{
		if (nouvelleSalle.m_connexions[i].estConnectee())  // Marqueur créé par la fonction essayerConnecterSalle
		{
			nouvelleSalle.m_connexions[i].deconnecter();

			for (int j(0); j < m_connexions.size(); j++)
			{
				if (m_connexions[j].connexionCompatible(nouvelleSalle.m_connexions[i]) && !m_connexions[j].estConnectee())
				{
					xCentreA = m_x + m_connexions[j].getX() + m_connexions[j].getW() / 2;
					yCentreA = m_y + m_connexions[j].getY() + m_connexions[j].getH() / 2;
					xCentreB = nouvelleSalle.m_x + nouvelleSalle.m_connexions[i].getX() + nouvelleSalle.m_connexions[i].getW() / 2;
					yCentreB = nouvelleSalle.m_y + nouvelleSalle.m_connexions[i].getY() + nouvelleSalle.m_connexions[i].getH() / 2;

					if (sqrt(pow(xCentreA - xCentreB, 2) + pow(yCentreA - yCentreB, 2)) <= DistanceFavorableConnexion)
					{
						nouvelleSalle.m_connexions[i].connecter(m_connexions[j]);
						nouvelleSalle.m_voisines.push_back(this);
						m_voisines.push_back(&nouvelleSalle);
					}
				}
			}
			break;
		}
	}
}


void Salle::deconnecterSalle(Salle& salleConnectee)
{
	for (int i(0); i < m_connexions.size(); i++)
	{
		for (int j(0); j < salleConnectee.m_connexions.size(); j++)
		{
			if (m_connexions[i].getSalleConnectee() == &(salleConnectee.m_connexions[j]))
			{
				m_connexions[i].deconnecter();
			}
		}
	}
}


bool Salle::raccorderSalle(Salle& salleConnectee)
{
	for (int i(0); i < m_voisines.size(); i++)
	{
		if (m_voisines[i] == &salleConnectee)
		{
			return false;
		}
	}

	int connexionIn(-1), connexionOut(-1);

	for (int i(0); i < m_connexions.size(); i++)
	{
		for (int j(0); j < salleConnectee.m_connexions.size(); j++)
		{
			if (m_connexions[i].connexionCompatible(salleConnectee.m_connexions[j]))
			{
				int xIn, yIn, xOut, yOut;

				if (m_connexions[i].getDirection() == Connexion::Direction::Bas)
				{
					xIn = m_x + m_connexions[i].getX() + m_connexions[i].getW() / 2;
					yIn = m_y + m_h;
					xOut = salleConnectee.m_x + salleConnectee.m_connexions[j].getX() + m_connexions[i].getW() / 2;
					yOut = salleConnectee.m_y;
				}
				else if (m_connexions[i].getDirection() == Connexion::Direction::Haut)
				{
					xIn = m_x + m_connexions[i].getX() + m_connexions[i].getW() / 2;
					yIn = m_y;
					xOut = salleConnectee.m_x + salleConnectee.m_connexions[j].getX() + m_connexions[i].getW() / 2;
					yOut = salleConnectee.m_y + salleConnectee.m_h;
				}
				else if (m_connexions[i].getDirection() == Connexion::Direction::Droite)
				{
					xIn = m_x + m_w;
					yIn = m_y + m_connexions[i].getY() + m_connexions[i].getH() / 2;
					xOut = salleConnectee.m_x;
					yOut = salleConnectee.m_y + salleConnectee.m_connexions[j].getY() + m_connexions[i].getH() / 2;
				}
				else if (m_connexions[i].getDirection() == Connexion::Direction::Gauche)
				{
					xIn = m_x;
					yIn = m_y + m_connexions[i].getY() + m_connexions[i].getH() / 2;
					xOut = salleConnectee.m_x + salleConnectee.m_w;
					yOut = salleConnectee.m_y + salleConnectee.m_connexions[j].getY() + m_connexions[i].getH() / 2;
				}

				if (xIn == xOut && yIn == yOut)
				{
					connexionIn = i;
					connexionOut = j;
					break;
				}
			}
		}

		if (connexionIn != -1)
		{
			break;
		}
	}

	if (connexionIn != -1)
	{
		salleConnectee.m_connexions[connexionOut].connecter(m_connexions[connexionIn]);
		salleConnectee.m_voisines.push_back(this);
		m_voisines.push_back(&salleConnectee);
	}

	return (connexionIn != -1);
}


void Salle::updateTexture(Machine* machinePointee)
{
	if (m_texture.getSize() != sf::Vector2u(m_w, m_h))
	{
		m_texture.create(m_w, m_h);
	}

	m_texture.clear(sf::Color(0, 0, 0, 0));

	sf::Sprite sprite(*(m_textureSalle));
	sprite.setPosition(0, 0);
	m_texture.draw(sprite);

	for (int i(0); i < m_machines.size(); i++)
	{
		sf::Sprite sprite(*m_machines[i]->getTexture());
		sprite.setPosition(m_machines[i]->getX(), m_machines[i]->getY());
		if (m_machines[i] == machinePointee)
		{
			sprite.setColor(sf::Color(0, 255, 0));
		}
		m_texture.draw(sprite);
	}

	for (int i(0); i < m_connexions.size(); i++)
	{
		if (m_connexions[i].estConnectee())
		{
			sf::IntRect rect(m_connexions[i].getX(), m_connexions[i].getY(), m_connexions[i].getW(), m_connexions[i].getH());
			sf::Sprite sprite(*(m_textureConnexions), rect);
			sprite.setPosition(m_connexions[i].getX(), m_connexions[i].getY());

			m_texture.draw(sprite);
		}
	}

	m_texture.display();
}


void Salle::afficher(Loader& loader, sf::RenderWindow& window, std::vector<int>& flags, float temperature, Machine* machinePointee)
{
	// Mettre à jour la texture brute de la salle

	updateTexture(machinePointee);

	// Calculer les valeurs à envoyer au shader

	float flagsEnvoyes[Salle::NombreDeFlags];
	for (int i(0); i < Salle::NombreDeFlags; i++)
	{
		flagsEnvoyes[i] = 0.0;
	}
	for (int i(0); i < flags.size(); i++)
	{
		flagsEnvoyes[flags[i]] = 1.0;
	}

	// Parametrer le shader

	sf::Shader* shader(loader.obtenirShader("shaders/mainSalles.frag"));
	shader->setUniform("textureCourrante", sf::Shader::CurrentTexture);
	shader->setUniform("nombreDeFlags", Salle::NombreDeFlags);
	shader->setUniformArray("flags", flagsEnvoyes, Salle::NombreDeFlags);
	shader->setUniform("temperature", temperature);

	// Afficher la salle

	sf::Sprite sprite(m_texture.getTexture());
	sprite.setPosition(m_x, m_y);

	window.draw(sprite, shader);
}


void Salle::afficherNoeuds(sf::RenderWindow& window) const
{
	for (int i(0); i < m_noeuds.size(); i++)
	{
		sf::CircleShape cercle(5);
		cercle.setFillColor(sf::Color(28, 175, 39));
		cercle.setPosition(m_x + m_noeuds[i]->getX(), m_y + m_noeuds[i]->getY());
		window.draw(cercle);
	}
}


std::vector<Connexion>* Salle::getConnexions()
{
	return &m_connexions;
}


std::vector<Salle*>* Salle::getVoisines()
{
	return &m_voisines;
}


std::vector<Machine*>* Salle::getMachines()
{
	return &m_machines;
}


Noeud* Salle::getNoeudNormal()
{
	for (int i(0); i < m_noeuds.size(); i++)
	{
		if (m_noeuds[i]->getType() == Noeud::Type::Normal)
		{
			return m_noeuds[i];
		}
	}

	return 0;
}


float Salle::getEnergieConso() const
{
	return m_energieConso;
}


float Salle::getEnergieConsoMachine() const
{
	float energie(0);
	for (int i(0); i < m_machines.size(); i++)
	{
		energie += m_machines[i]->getEnergieConso();
	}

	return energie;
}


float Salle::getEnergieProduite() const
{
	return m_energieProduite;
}


float Salle::getEnergieSolaireProduite() const
{
	return m_energieSolaireProduite;
}


float Salle::getCapaciteAccu() const
{
	return m_capaciteAccu;
}


float Salle::getEnergieAccu() const
{
	return m_energieAccu;
}


float Salle::getChaleurDissipee() const
{
	return m_chaleurDissipee;
}


float Salle::getChaleurDissipeeMachines() const
{
	float chaleur(0);
	for (int i(0); i < m_machines.size(); i++)
	{
		chaleur += m_machines[i]->getChaleurDissipee();
	}

	return chaleur;
}


bool Salle::getPanneElectrique() const
{
	return m_panneElectrique;
}


Salle::~Salle()
{
    for(int i(0); i < m_voisines.size(); i++)
    {
		m_voisines[i]->deconnecterSalle(*this);

        int k(-1);

        for(int j(0); j < (m_voisines[i]->m_voisines).size(); j++)
        {
            if((*m_voisines[i]).m_voisines[j] == this)
            {
                k = j;
            }
        }

        (*m_voisines[i]).m_voisines.erase((*m_voisines[i]).m_voisines.begin() + k);
    }

	for (int i(0); i < m_machines.size(); i++)
	{
		m_machines[i]->setNoeudProche(0);
		delete m_machines[i];
	}

	for (int i(0); i < m_noeuds.size(); i++)
	{
		delete m_noeuds[i];
	}
}


// FONCTIONS MEMBRES DE LA CLASSE SALLE_COMMANDES


SalleCommandes::SalleCommandes(Loader& loader)
{
	m_energieConso = 2;
	m_energieProduite = 10;
	m_chaleurDissipee = 5;

	m_textureSalle = loader.obtenirTexture("images/salle des commandes/salle.png");
	m_textureConnexions = loader.obtenirTexture("images/salle des commandes/connexion.png");
	m_root = true;

	m_w = (m_textureSalle->getSize()).x;
	m_h = (m_textureSalle->getSize()).y;
	m_x = -m_w / 2;
	m_y = -m_h / 2;

	// Noeuds

	Noeud *noeudMilieu(new Noeud), *noeudConnexion(new Noeud);

	noeudMilieu->setCoord(m_w / 2, 155);
	noeudMilieu->setType(Noeud::Type::Normal);

	noeudConnexion->setCoord(m_w / 2, m_h);
	noeudConnexion->connecter(noeudMilieu);
	noeudConnexion->setType(Noeud::Type::Connexion);

	m_noeuds.push_back(noeudMilieu);
	m_noeuds.push_back(noeudConnexion);

	// Connexions

	Connexion c;
	c.setCoord((m_w - 76) / 2, m_h - 16, 76, 16);
	c.setDirection(Connexion::Direction::Bas);
	c.setNoeud(noeudConnexion);
	m_connexions.push_back(c);
}


// FONCTIONS MEMBRES DE LA CLASSE COULOIR_I


CouloirI::CouloirI(Loader& loader)
{
	m_positionRotation = Salle::Orientation::Verticale;

	m_textureSalle = loader.obtenirTexture("images/couloir I/vertical/salle.png");
	m_textureConnexions = loader.obtenirTexture("images/couloir I/vertical/connexions.png");

	m_w = (m_textureSalle->getSize()).x;
	m_h = (m_textureSalle->getSize()).y;
	m_x = -m_w / 2;
	m_y = -m_h / 2;

	// Noeuds

	Noeud *noeudMilieu(new Noeud), *noeudConnexionHaut(new Noeud), *noeudConnexionBas(new Noeud);

	noeudMilieu->setCoord(m_w / 2, m_h / 2);
	noeudMilieu->setType(Noeud::Type::Normal);

	noeudConnexionHaut->setCoord(m_w / 2, 0);
	noeudConnexionHaut->connecter(noeudMilieu);
	noeudConnexionHaut->setType(Noeud::Type::Connexion);

	noeudConnexionBas->setCoord(m_w / 2, m_h);
	noeudConnexionBas->connecter(noeudMilieu);
	noeudConnexionBas->setType(Noeud::Type::Connexion);

	m_noeuds.push_back(noeudMilieu);
	m_noeuds.push_back(noeudConnexionHaut);
	m_noeuds.push_back(noeudConnexionBas);

	// Connexions

	Connexion cBas, cHaut;

	cBas.setCoord((m_w - 76) / 2, m_h - 16, 76, 16);
	cBas.setDirection(Connexion::Direction::Bas);
	cBas.setNoeud(noeudConnexionBas);
	m_connexions.push_back(cBas);

	cHaut.setCoord((m_w - 76) / 2, 0, 76, 16);
	cHaut.setDirection(Connexion::Direction::Haut);
	cHaut.setNoeud(noeudConnexionHaut);
	m_connexions.push_back(cHaut);
}


void CouloirI::effectuerRotation(Loader& loader)
{
	if (m_positionRotation == Salle::Orientation::Verticale)
	{
		m_positionRotation = Salle::Orientation::Horizontale;

		m_textureSalle = loader.obtenirTexture("images/couloir I/horizontal/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/couloir I/horizontal/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;

		// Noeuds

		m_noeuds[0]->setCoord(m_w / 2, m_h / 2);

		m_noeuds[1]->setCoord(0, m_h / 2);
		m_noeuds[2]->setCoord(m_w, m_h / 2);

		// Connexions

		Connexion cGauche, cDroite;

		cGauche.setCoord(0, (m_h - 76) / 2, 16, 76);
		cGauche.setDirection(Connexion::Direction::Gauche);
		cGauche.setNoeud(m_noeuds[1]);
		m_connexions[0] = cGauche;

		cDroite.setCoord(m_w - 16, (m_h - 76) / 2, 16, 76);
		cDroite.setDirection(Connexion::Direction::Droite);
		cDroite.setNoeud(m_noeuds[2]);
		m_connexions[1] = cDroite;
	}
	else
	{
		m_positionRotation = Salle::Orientation::Verticale;

		m_textureSalle = loader.obtenirTexture("images/couloir I/vertical/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/couloir I/vertical/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;

		// Noeuds

		m_noeuds[0]->setCoord(m_w / 2, m_h / 2);

		m_noeuds[1]->setCoord(m_w / 2, 0);
		m_noeuds[2]->setCoord(m_w / 2, m_h);

		// Connexions

		Connexion cBas, cHaut;

		cBas.setCoord((m_w - 76) / 2, m_h - 16, 76, 16);
		cBas.setDirection(Connexion::Direction::Bas);
		cBas.setNoeud(m_noeuds[2]);
		m_connexions[0] = cBas;

		cHaut.setCoord((m_w - 76) / 2, 0, 76, 16);
		cHaut.setDirection(Connexion::Direction::Haut);
		cHaut.setNoeud(m_noeuds[1]);
		m_connexions[1] = cHaut;
	}
}


// FONCTIONS MEMBRES DE LA CLASSE COULOIR_T


CouloirT::CouloirT(Loader& loader)
{
	m_energieConso = 1.5;

	m_positionRotation = Salle::Orientation::Haut;

	m_textureSalle = loader.obtenirTexture("images/couloir T/haut/salle.png");
	m_textureConnexions = loader.obtenirTexture("images/couloir T/haut/connexions.png");

	m_w = (m_textureSalle->getSize()).x;
	m_h = (m_textureSalle->getSize()).y;
	m_x = -m_w / 2;
	m_y = -m_h / 2;

	// Noeuds

	Noeud *noeudMilieu(new Noeud), *noeudConnexionHaut(new Noeud), *noeudConnexionDroite(new Noeud), *noeudConnexionGauche(new Noeud);

	noeudMilieu->setCoord(m_w / 2, 100);
	noeudMilieu->setType(Noeud::Type::Normal);

	noeudConnexionHaut->setCoord(m_w / 2, 0);
	noeudConnexionHaut->connecter(noeudMilieu);
	noeudConnexionHaut->setType(Noeud::Type::Connexion);

	noeudConnexionDroite->setCoord(m_w, 100);
	noeudConnexionDroite->connecter(noeudMilieu);
	noeudConnexionDroite->setType(Noeud::Type::Connexion);

	noeudConnexionGauche->setCoord(0, 100);
	noeudConnexionGauche->connecter(noeudMilieu);
	noeudConnexionGauche->setType(Noeud::Type::Connexion);

	m_noeuds.push_back(noeudMilieu);
	m_noeuds.push_back(noeudConnexionHaut);
	m_noeuds.push_back(noeudConnexionDroite);
	m_noeuds.push_back(noeudConnexionGauche);

	// Connexions

	Connexion cHaut, cDroite, cGauche;

	cHaut.setCoord((m_w - 76) / 2, 0, 76, 16);
	cHaut.setDirection(Connexion::Direction::Haut);
	cHaut.setNoeud(noeudConnexionHaut);
	m_connexions.push_back(cHaut);

	cDroite.setCoord(184, 62, 16, 76);
	cDroite.setDirection(Connexion::Direction::Droite);
	cDroite.setNoeud(noeudConnexionDroite);
	m_connexions.push_back(cDroite);

	cGauche.setCoord(0, 62, 16, 76);
	cGauche.setDirection(Connexion::Direction::Gauche);
	cGauche.setNoeud(noeudConnexionGauche);
	m_connexions.push_back(cGauche);
}


void CouloirT::effectuerRotation(Loader& loader)
{

	if (m_positionRotation == Salle::Orientation::Haut)
	{
		m_positionRotation = Salle::Orientation::Droite;

		m_textureSalle = loader.obtenirTexture("images/couloir T/droit/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/couloir T/droit/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;
		m_x = -m_w / 2;
		m_y = -m_h / 2;

		// Noeuds

		m_noeuds[0]->setCoord(42, m_h / 2);

		m_noeuds[1]->setCoord(m_w, m_h / 2);
		m_noeuds[2]->setCoord(42, 0);
		m_noeuds[3]->setCoord(42, m_h);

		// Connexions

		Connexion cDroite, cHaut, cBas;

		cDroite.setCoord(126, 62, 16, 76);
		cDroite.setDirection(Connexion::Direction::Droite);
		cDroite.setNoeud(m_noeuds[1]);
		m_connexions[0] = cDroite;

		cHaut.setCoord(4, 0, 76, 16);
		cHaut.setDirection(Connexion::Direction::Haut);
		cHaut.setNoeud(m_noeuds[2]);
		m_connexions[1] = cHaut;

		cBas.setCoord(4, 184, 76, 16);
		cBas.setDirection(Connexion::Direction::Bas);
		cBas.setNoeud(m_noeuds[3]);
		m_connexions[2] = cBas;
	}
	else if (m_positionRotation == Salle::Orientation::Droite)
	{
		m_positionRotation = Salle::Orientation::Bas;

		m_textureSalle = loader.obtenirTexture("images/couloir T/bas/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/couloir T/bas/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;
		m_x = -m_w / 2;
		m_y = -m_h / 2;

		// Noeuds

		m_noeuds[0]->setCoord(m_w / 2, 42);

		m_noeuds[1]->setCoord(m_w / 2, m_h);
		m_noeuds[2]->setCoord(m_w, 42);
		m_noeuds[3]->setCoord(0, 42);

		// Connexions

		Connexion cBas, cDroite, cGauche;

		cBas.setCoord(62, 126, 76, 16);
		cBas.setDirection(Connexion::Direction::Bas);
		cBas.setNoeud(m_noeuds[1]);
		m_connexions[0] = cBas;

		cDroite.setCoord(184, 4, 16, 76);
		cDroite.setDirection(Connexion::Direction::Droite);
		cDroite.setNoeud(m_noeuds[2]);
		m_connexions[1] = cDroite;

		cGauche.setCoord(0, 4, 16, 76);
		cGauche.setDirection(Connexion::Direction::Gauche);
		cGauche.setNoeud(m_noeuds[3]);
		m_connexions[2] = cGauche;
	}
	else if (m_positionRotation == Salle::Orientation::Bas)
	{
		m_positionRotation = Salle::Orientation::Gauche;

		m_textureSalle = loader.obtenirTexture("images/couloir T/gauche/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/couloir T/gauche/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;
		m_x = -m_w / 2;
		m_y = -m_h / 2;

		// Noeuds

		m_noeuds[0]->setCoord(100, m_h / 2);

		m_noeuds[1]->setCoord(0, m_h / 2);
		m_noeuds[2]->setCoord(100, 0);
		m_noeuds[3]->setCoord(100, m_h);

		// Connexions

		Connexion cGauche, cHaut, cBas;

		cGauche.setCoord(0, 62, 16, 76);
		cGauche.setDirection(Connexion::Direction::Gauche);
		cGauche.setNoeud(m_noeuds[1]);
		m_connexions[0] = cGauche;

		cHaut.setCoord(62, 0, 76, 16);
		cHaut.setDirection(Connexion::Direction::Haut);
		cHaut.setNoeud(m_noeuds[2]);
		m_connexions[1] = cHaut;

		cBas.setCoord(62, 184, 76, 16);
		cBas.setDirection(Connexion::Direction::Bas);
		cBas.setNoeud(m_noeuds[3]);
		m_connexions[2] = cBas;
	}
	else
	{
		m_positionRotation = Salle::Orientation::Haut;

		m_textureSalle = loader.obtenirTexture("images/couloir T/haut/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/couloir T/haut/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;
		m_x = -m_w / 2;
		m_y = -m_h / 2;

		// Noeuds

		m_noeuds[0]->setCoord(m_w / 2, 100);

		m_noeuds[1]->setCoord(m_w / 2, 0);
		m_noeuds[2]->setCoord(m_w, 100);
		m_noeuds[3]->setCoord(0, 100);

		// Connexions

		Connexion cHaut, cDroite, cGauche;

		cHaut.setCoord((m_w - 76) / 2, 0, 76, 16);
		cHaut.setDirection(Connexion::Direction::Haut);
		cHaut.setNoeud(m_noeuds[1]);
		m_connexions[0] = cHaut;

		cDroite.setCoord(184, 62, 16, 76);
		cDroite.setDirection(Connexion::Direction::Droite);
		cDroite.setNoeud(m_noeuds[2]);
		m_connexions[1] = cDroite;

		cGauche.setCoord(0, 62, 16, 76);
		cGauche.setDirection(Connexion::Direction::Gauche);
		cGauche.setNoeud(m_noeuds[3]);
		m_connexions[2] = cGauche;
	}
}


// FONCTIONS MEMBRES DE LA CLASSE COULOIR_L


CouloirL::CouloirL(Loader& loader)
{
	m_positionRotation = Salle::Orientation::Haut;

	m_textureSalle = loader.obtenirTexture("images/couloir L/haut/salle.png");
	m_textureConnexions = loader.obtenirTexture("images/couloir L/haut/connexions.png");

	m_w = (m_textureSalle->getSize()).x;
	m_h = (m_textureSalle->getSize()).y;
	m_x = -m_w / 2;
	m_y = -m_h / 2;

	// Noeuds

	Noeud *noeudMilieu(new Noeud), *noeudConnexionHaut(new Noeud), *noeudConnexionDroite(new Noeud);

	noeudMilieu->setCoord(42, 100);
	noeudMilieu->setType(Noeud::Type::Normal);

	noeudConnexionHaut->setCoord(42, 0);
	noeudConnexionHaut->connecter(noeudMilieu);
	noeudConnexionHaut->setType(Noeud::Type::Connexion);

	noeudConnexionDroite->setCoord(m_w, 100);
	noeudConnexionDroite->connecter(noeudMilieu);
	noeudConnexionDroite->setType(Noeud::Type::Connexion);

	m_noeuds.push_back(noeudMilieu);
	m_noeuds.push_back(noeudConnexionHaut);
	m_noeuds.push_back(noeudConnexionDroite);

	// Connexions

	Connexion cHaut, cDroite;

	cHaut.setCoord(4, 0, 76, 16);
	cHaut.setDirection(Connexion::Direction::Haut);
	cHaut.setNoeud(noeudConnexionHaut);
	m_connexions.push_back(cHaut);

	cDroite.setCoord(126, 62, 16, 76);
	cDroite.setDirection(Connexion::Direction::Droite);
	cDroite.setNoeud(noeudConnexionDroite);
	m_connexions.push_back(cDroite);
}


void CouloirL::effectuerRotation(Loader& loader)
{

	if (m_positionRotation == Salle::Orientation::Haut)
	{
		m_positionRotation = Salle::Orientation::Droite;

		m_textureSalle = loader.obtenirTexture("images/couloir L/droit/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/couloir L/droit/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;
		m_x = -m_w / 2;
		m_y = -m_h / 2;

		// Noeuds

		m_noeuds[0]->setCoord(42, 42);

		m_noeuds[1]->setCoord(m_w, 42);
		m_noeuds[2]->setCoord(42, m_h);

		// Connexions

		Connexion cDroite, cBas;

		cDroite.setCoord(126, 4, 16, 76);
		cDroite.setDirection(Connexion::Direction::Droite);
		cDroite.setNoeud(m_noeuds[1]);
		m_connexions[0] = cDroite;

		cBas.setCoord(4, 126, 76, 16);
		cBas.setDirection(Connexion::Direction::Bas);
		cBas.setNoeud(m_noeuds[2]);
		m_connexions[1] = cBas;
	}
	else if (m_positionRotation == Salle::Orientation::Droite)
	{
		m_positionRotation = Salle::Orientation::Bas;

		m_textureSalle = loader.obtenirTexture("images/couloir L/bas/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/couloir L/bas/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;
		m_x = -m_w / 2;
		m_y = -m_h / 2;

		// Noeuds

		m_noeuds[0]->setCoord(100, 42);

		m_noeuds[1]->setCoord(100, m_h);
		m_noeuds[2]->setCoord(0, 42);

		// Connexions

		Connexion cBas, cGauche;

		cBas.setCoord(62, 126, 76, 16);
		cBas.setDirection(Connexion::Direction::Bas);
		cBas.setNoeud(m_noeuds[1]);
		m_connexions[0] = cBas;

		cGauche.setCoord(0, 4, 16, 76);
		cGauche.setDirection(Connexion::Direction::Gauche);
		cGauche.setNoeud(m_noeuds[2]);
		m_connexions[1] = cGauche;
	}
	else if (m_positionRotation == Salle::Orientation::Bas)
	{
		m_positionRotation = Salle::Orientation::Gauche;

		m_textureSalle = loader.obtenirTexture("images/couloir L/gauche/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/couloir L/gauche/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;
		m_x = -m_w / 2;
		m_y = -m_h / 2;

		// Noeuds

		m_noeuds[0]->setCoord(100, 100);

		m_noeuds[1]->setCoord(0, 100);
		m_noeuds[2]->setCoord(100, 0);

		// Connexions

		Connexion cGauche, cHaut;

		cGauche.setCoord(0, 62, 16, 76);
		cGauche.setDirection(Connexion::Direction::Gauche);
		cGauche.setNoeud(m_noeuds[1]);
		m_connexions[0] = cGauche;

		cHaut.setCoord(62, 0, 76, 16);
		cHaut.setDirection(Connexion::Direction::Haut);
		cHaut.setNoeud(m_noeuds[2]);
		m_connexions[1] = cHaut;
	}
	else
	{
		m_positionRotation = Salle::Orientation::Haut;

		m_textureSalle = loader.obtenirTexture("images/couloir L/haut/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/couloir L/haut/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;
		m_x = -m_w / 2;
		m_y = -m_h / 2;

		// Noeuds

		m_noeuds[0]->setCoord(42, 100);

		m_noeuds[1]->setCoord(42, 0);
		m_noeuds[2]->setCoord(142, 100);

		// Connexions

		Connexion cHaut, cDroite;

		cHaut.setCoord(4, 0, 76, 16);
		cHaut.setDirection(Connexion::Direction::Haut);
		cHaut.setNoeud(m_noeuds[1]);
		m_connexions[0] = cHaut;

		cDroite.setCoord(126, 62, 16, 76);
		cDroite.setDirection(Connexion::Direction::Droite);
		cDroite.setNoeud(m_noeuds[2]);
		m_connexions[1] = cDroite;
	}
}


// FONCTIONS MEMBRES DE LA CLASSE COULOIR_X


CouloirX::CouloirX(Loader& loader)
{
	m_energieConso = 2;

	m_textureSalle = loader.obtenirTexture("images/couloir X/salle.png");
	m_textureConnexions = loader.obtenirTexture("images/couloir X/connexions.png");

	m_w = (m_textureSalle->getSize()).x;
	m_h = (m_textureSalle->getSize()).y;
	m_x = -m_w / 2;
	m_y = -m_h / 2;

	// Noeuds

	Noeud *noeudMilieu(new Noeud), *noeudConnexionHaut(new Noeud), *noeudConnexionDroite(new Noeud), *noeudConnexionGauche(new Noeud), *noeudConnexionBas(new Noeud);

	noeudMilieu->setCoord(100, 100);
	noeudMilieu->setType(Noeud::Type::Normal);

	noeudConnexionHaut->setCoord(100, 0);
	noeudConnexionHaut->connecter(noeudMilieu);
	noeudConnexionHaut->setType(Noeud::Type::Connexion);

	noeudConnexionDroite->setCoord(200, 100);
	noeudConnexionDroite->connecter(noeudMilieu);
	noeudConnexionDroite->setType(Noeud::Type::Connexion);

	noeudConnexionGauche->setCoord(0, 100);
	noeudConnexionGauche->connecter(noeudMilieu);
	noeudConnexionGauche->setType(Noeud::Type::Connexion);

	noeudConnexionBas->setCoord(100, 200);
	noeudConnexionBas->connecter(noeudMilieu);
	noeudConnexionBas->setType(Noeud::Type::Connexion);

	m_noeuds.push_back(noeudMilieu);
	m_noeuds.push_back(noeudConnexionHaut);
	m_noeuds.push_back(noeudConnexionDroite);
	m_noeuds.push_back(noeudConnexionGauche);
	m_noeuds.push_back(noeudConnexionBas);

	// Connexions

	Connexion cHaut, cDroite, cGauche, cBas;

	cHaut.setCoord(62, 0, 76, 16);
	cHaut.setDirection(Connexion::Direction::Haut);
	cHaut.setNoeud(noeudConnexionHaut);
	m_connexions.push_back(cHaut);

	cDroite.setCoord(184, 62, 16, 76);
	cDroite.setDirection(Connexion::Direction::Droite);
	cDroite.setNoeud(noeudConnexionDroite);
	m_connexions.push_back(cDroite);

	cGauche.setCoord(0, 62, 16, 76);
	cGauche.setDirection(Connexion::Direction::Gauche);
	cGauche.setNoeud(noeudConnexionGauche);
	m_connexions.push_back(cGauche);

	cBas.setCoord(62, 184, 76, 16);
	cBas.setDirection(Connexion::Direction::Bas);
	cBas.setNoeud(noeudConnexionBas);
	m_connexions.push_back(cBas);
}


// FONCTIONS MEMBRES DE LA CLASSE SALLE_DE_BATTERIES


SalleDeBatteries::SalleDeBatteries(Loader& loader)
{
	m_energieConso = 2;
	m_capaciteAccu = 1000;

	m_positionRotation = Salle::Orientation::Verticale;

	m_textureSalle = loader.obtenirTexture("images/salle de batteries/verticale/salle.png");
	m_textureConnexions = loader.obtenirTexture("images/salle de batteries/verticale/connexions.png");

	m_w = (m_textureSalle->getSize()).x;
	m_h = (m_textureSalle->getSize()).y;
	m_x = -m_w / 2;
	m_y = -m_h / 2;

	// Noeuds

	Noeud *noeudMilieu(new Noeud), *noeudConnexionHaut(new Noeud), *noeudConnexionBas(new Noeud);

	noeudMilieu->setCoord(m_w / 2, m_h / 2);
	noeudMilieu->setType(Noeud::Type::Normal);

	noeudConnexionHaut->setCoord(m_w / 2, 0);
	noeudConnexionHaut->connecter(noeudMilieu);
	noeudConnexionHaut->setType(Noeud::Type::Connexion);

	noeudConnexionBas->setCoord(m_w / 2, m_h);
	noeudConnexionBas->connecter(noeudMilieu);
	noeudConnexionBas->setType(Noeud::Type::Connexion);

	m_noeuds.push_back(noeudMilieu);
	m_noeuds.push_back(noeudConnexionHaut);
	m_noeuds.push_back(noeudConnexionBas);

	// Connexions

	Connexion cBas, cHaut;

	cBas.setCoord((m_w - 76) / 2, m_h - 16, 76, 16);
	cBas.setDirection(Connexion::Direction::Bas);
	cBas.setNoeud(noeudConnexionBas);
	m_connexions.push_back(cBas);

	cHaut.setCoord((m_w - 76) / 2, 0, 76, 16);
	cHaut.setDirection(Connexion::Direction::Haut);
	cHaut.setNoeud(noeudConnexionHaut);
	m_connexions.push_back(cHaut);
}


void SalleDeBatteries::effectuerRotation(Loader& loader)
{
	if (m_positionRotation == Salle::Orientation::Verticale)
	{
		m_positionRotation = Salle::Orientation::Horizontale;

		m_textureSalle = loader.obtenirTexture("images/salle de batteries/horizontale/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/salle de batteries/horizontale/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;

		// Noeuds

		m_noeuds[0]->setCoord(m_w / 2, m_h / 2);

		m_noeuds[1]->setCoord(0, m_h / 2);
		m_noeuds[2]->setCoord(m_w, m_h / 2);

		// Connexions

		Connexion cGauche, cDroite;

		cGauche.setCoord(0, (m_h - 76) / 2, 16, 76);
		cGauche.setDirection(Connexion::Direction::Gauche);
		cGauche.setNoeud(m_noeuds[1]);
		m_connexions[0] = cGauche;

		cDroite.setCoord(m_w - 16, (m_h - 76) / 2, 16, 76);
		cDroite.setDirection(Connexion::Direction::Droite);
		cDroite.setNoeud(m_noeuds[2]);
		m_connexions[1] = cDroite;
	}
	else
	{
		m_positionRotation = Salle::Orientation::Verticale;

		m_textureSalle = loader.obtenirTexture("images/salle de batteries/verticale/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/salle de batteries/verticale/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;

		// Noeuds

		m_noeuds[0]->setCoord(m_w / 2, m_h / 2);

		m_noeuds[1]->setCoord(m_w / 2, m_h);
		m_noeuds[2]->setCoord(m_w / 2, 0);

		// Connexions

		Connexion cBas, cHaut;

		cBas.setCoord((m_w - 76) / 2, m_h - 16, 76, 16);
		cBas.setDirection(Connexion::Direction::Bas);
		cBas.setNoeud(m_noeuds[1]);
		m_connexions[0] = cBas;

		cHaut.setCoord((m_w - 76) / 2, 0, 76, 16);
		cHaut.setDirection(Connexion::Direction::Haut);
		cHaut.setNoeud(m_noeuds[2]);
		m_connexions[1] = cHaut;
	}
}


// FONCTIONS MEMBRES DE LA CLASSE CENTRALE_ELECTRIQUE


CentraleElectrique::CentraleElectrique(Loader& loader)
{
	m_energieProduite = 10;
	m_energieSolaireProduite = 10;
	m_energieConso = 2;

	m_positionRotation = Salle::Orientation::Verticale;

	m_textureSalle = loader.obtenirTexture("images/centrale electrique/verticale/salle.png");
	m_textureConnexions = loader.obtenirTexture("images/centrale electrique/verticale/connexions.png");

	m_w = (m_textureSalle->getSize()).x;
	m_h = (m_textureSalle->getSize()).y;
	m_x = -m_w / 2;
	m_y = -m_h / 2;

	// Noeuds

	Noeud *noeudMilieu(new Noeud), *noeudConnexionHaut(new Noeud), *noeudConnexionBas(new Noeud);

	noeudMilieu->setCoord(m_w / 2, m_h / 2);
	noeudMilieu->setType(Noeud::Type::Normal);

	noeudConnexionHaut->setCoord(m_w / 2, 0);
	noeudConnexionHaut->connecter(noeudMilieu);
	noeudConnexionHaut->setType(Noeud::Type::Connexion);

	noeudConnexionBas->setCoord(m_w / 2, m_h);
	noeudConnexionBas->connecter(noeudMilieu);
	noeudConnexionBas->setType(Noeud::Type::Connexion);

	m_noeuds.push_back(noeudMilieu);
	m_noeuds.push_back(noeudConnexionHaut);
	m_noeuds.push_back(noeudConnexionBas);

	// Connexions

	Connexion cBas, cHaut, cGauche, cDroite;

	cBas.setCoord((m_w - 76) / 2, m_h - 16, 76, 16);
	cBas.setDirection(Connexion::Direction::Bas);
	cBas.setNoeud(noeudConnexionBas);
	m_connexions.push_back(cBas);

	cHaut.setCoord((m_w - 76) / 2, 0, 76, 16);
	cHaut.setDirection(Connexion::Direction::Haut);
	cHaut.setNoeud(noeudConnexionHaut);
	m_connexions.push_back(cHaut);

	cGauche.setCoord(0, (m_h - 10) / 2, 16, 10);
	cGauche.setDirection(Connexion::Direction::Gauche);
	cGauche.setType(Connexion::Type::PanneauSolaire);
	m_connexions.push_back(cGauche);

	cDroite.setCoord(m_w - 16, (m_h - 10) / 2, 16, 10);
	cDroite.setDirection(Connexion::Direction::Droite);
	cDroite.setType(Connexion::Type::PanneauSolaire);
	m_connexions.push_back(cDroite);
}


void CentraleElectrique::effectuerRotation(Loader& loader)
{
	if (m_positionRotation == Salle::Orientation::Verticale)
	{
		m_positionRotation = Salle::Orientation::Horizontale;

		m_textureSalle = loader.obtenirTexture("images/centrale electrique/horizontale/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/centrale electrique/horizontale/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;

		// Noeuds

		m_noeuds[0]->setCoord(m_w / 2, m_h / 2);

		m_noeuds[1]->setCoord(0, m_h / 2);
		m_noeuds[2]->setCoord(m_w, m_h / 2);

		// Connexions

		Connexion cBas, cHaut, cGauche, cDroite;

		cBas.setCoord((m_w - 10) / 2, m_h - 16, 10, 16);
		cBas.setDirection(Connexion::Direction::Bas);
		cBas.setType(Connexion::Type::PanneauSolaire);
		m_connexions[0] = cBas;

		cHaut.setCoord((m_w - 10) / 2, 0, 10, 16);
		cHaut.setDirection(Connexion::Direction::Haut);
		cHaut.setType(Connexion::Type::PanneauSolaire);
		m_connexions[1] = cHaut;

		cGauche.setCoord(0, (m_h - 76) / 2, 16, 76);
		cGauche.setDirection(Connexion::Direction::Gauche);
		cGauche.setNoeud(m_noeuds[1]);
		m_connexions[2] = cGauche;

		cDroite.setCoord(m_w - 16, (m_h - 76) / 2, 16, 76);
		cDroite.setDirection(Connexion::Direction::Droite);
		cDroite.setNoeud(m_noeuds[2]);
		m_connexions[3] = cDroite;
	}
	else
	{
		m_positionRotation = Salle::Orientation::Verticale;

		m_textureSalle = loader.obtenirTexture("images/centrale electrique/verticale/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/centrale electrique/verticale/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;

		// Noeuds

		m_noeuds[0]->setCoord(m_w / 2, m_h / 2);

		m_noeuds[1]->setCoord(m_w / 2, m_h);
		m_noeuds[2]->setCoord(m_w / 2, 0);

		// Connexions

		Connexion cBas, cHaut, cGauche, cDroite;

		cBas.setCoord((m_w - 76) / 2, m_h - 16, 76, 16);
		cBas.setDirection(Connexion::Direction::Bas);
		cBas.setNoeud(m_noeuds[1]);
		m_connexions[0] = cBas;

		cHaut.setCoord((m_w - 76) / 2, 0, 76, 16);
		cHaut.setDirection(Connexion::Direction::Haut);
		cHaut.setNoeud(m_noeuds[2]);
		m_connexions[1] = cHaut;

		cGauche.setCoord(0, (m_h - 10) / 2, 16, 10);
		cGauche.setDirection(Connexion::Direction::Gauche);
		cGauche.setType(Connexion::Type::PanneauSolaire);
		m_connexions[2] = cGauche;

		cDroite.setCoord(m_w - 16, (m_h - 10) / 2, 16, 10);
		cDroite.setDirection(Connexion::Direction::Droite);
		cDroite.setType(Connexion::Type::PanneauSolaire);
		m_connexions[3] = cDroite;
	}
}


// FONCTIONS MEMBRES DE LA CLASSE PANNEAU_SOLAIRE


PanneauSolaire::PanneauSolaire(Loader& loader)
{
	m_impasse = true;

	m_energieSolaireProduite = 40;

	m_positionRotation = Salle::Orientation::Horizontale;

	m_textureSalle = loader.obtenirTexture("images/panneau solaire/horizontal/salle.png");
	m_textureConnexions = loader.obtenirTexture("images/panneau solaire/horizontal/connexions.png");

	m_w = (m_textureSalle->getSize()).x;
	m_h = (m_textureSalle->getSize()).y;
	m_x = -m_w / 2;
	m_y = -m_h / 2;

	Connexion cGauche, cDroite;

	cGauche.setCoord(0, (m_h - 10) / 2, 16, 10);
	cGauche.setDirection(Connexion::Direction::Gauche);
	cGauche.setType(Connexion::Type::PanneauSolaire);
	m_connexions.push_back(cGauche);

	cDroite.setCoord(m_w - 16, (m_h - 10) / 2, 16, 10);
	cDroite.setDirection(Connexion::Direction::Droite);
	cDroite.setType(Connexion::Type::PanneauSolaire);
	m_connexions.push_back(cDroite);
}


void PanneauSolaire::effectuerRotation(Loader& loader)
{
	if (m_positionRotation == Salle::Orientation::Verticale)
	{
		m_positionRotation = Salle::Orientation::Horizontale;

		m_textureSalle = loader.obtenirTexture("images/panneau solaire/horizontal/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/panneau solaire/horizontal/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;

		Connexion cGauche, cDroite;

		cGauche.setCoord(0, (m_h - 10) / 2, 16, 10);
		cGauche.setDirection(Connexion::Direction::Gauche);
		cGauche.setType(Connexion::Type::PanneauSolaire);
		m_connexions[0] = cGauche;

		cDroite.setCoord(m_w - 16, (m_h - 10) / 2, 16, 10);
		cDroite.setDirection(Connexion::Direction::Droite);
		cDroite.setType(Connexion::Type::PanneauSolaire);
		m_connexions[1] = cDroite;
	}
	else
	{
		m_positionRotation = Salle::Orientation::Verticale;

		m_textureSalle = loader.obtenirTexture("images/panneau solaire/vertical/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/panneau solaire/vertical/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;

		Connexion cBas, cHaut;

		cBas.setCoord((m_w - 10) / 2, m_h - 16, 10, 16);
		cBas.setDirection(Connexion::Direction::Bas);
		cBas.setType(Connexion::Type::PanneauSolaire);
		m_connexions[0] = cBas;

		cHaut.setCoord((m_w - 10) / 2, 0, 10, 16);
		cHaut.setDirection(Connexion::Direction::Haut);
		cHaut.setType(Connexion::Type::PanneauSolaire);
		m_connexions[1] = cHaut;
	}
}


// FONCTIONS MEMBRES DE LA CLASSE PILE_RADIO_ISOTOPIQUE


PileRadioIsotopique::PileRadioIsotopique(Loader& loader)
{
	m_impasse = true;

	m_energieProduite = 20;

	m_positionRotation = Salle::Orientation::Haut;

	m_textureSalle = loader.obtenirTexture("images/pile radio isotopique/haut.png");
	m_textureConnexions = loader.obtenirTexture("images/pile radio isotopique/haut.png");

	m_w = (m_textureSalle->getSize()).x;
	m_h = (m_textureSalle->getSize()).y;
	m_x = -m_w / 2;
	m_y = -m_h / 2;

	Connexion cHaut;

	cHaut.setCoord((m_w - 10) / 2, 0, 10, 16);
	cHaut.setDirection(Connexion::Direction::Haut);
	cHaut.setType(Connexion::Type::PanneauSolaire);
	m_connexions.push_back(cHaut);
}


void PileRadioIsotopique::effectuerRotation(Loader& loader)
{
	if (m_positionRotation == Salle::Orientation::Haut)
	{
		m_positionRotation = Salle::Orientation::Droite;

		m_textureSalle = loader.obtenirTexture("images/pile radio isotopique/droite.png");
		m_textureConnexions = loader.obtenirTexture("images/pile radio isotopique/droite.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;
		m_x = -m_w / 2;
		m_y = -m_h / 2;

		// Connexions

		Connexion cDroite;

		cDroite.setCoord(m_w - 16, (m_h - 10) / 2, 16, 10);
		cDroite.setDirection(Connexion::Direction::Droite);
		cDroite.setType(Connexion::Type::PanneauSolaire);
		m_connexions[0] = cDroite;
	}
	else if (m_positionRotation == Salle::Orientation::Droite)
	{
		m_positionRotation = Salle::Orientation::Bas;

		m_textureSalle = loader.obtenirTexture("images/pile radio isotopique/bas.png");
		m_textureConnexions = loader.obtenirTexture("images/pile radio isotopique/bas.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;
		m_x = -m_w / 2;
		m_y = -m_h / 2;

		// Connexions

		Connexion cBas;

		cBas.setCoord((m_w - 10) / 2, m_h - 16, 10, 16);
		cBas.setDirection(Connexion::Direction::Bas);
		cBas.setType(Connexion::Type::PanneauSolaire);
		m_connexions[0] = cBas;
	}
	else if (m_positionRotation == Salle::Orientation::Bas)
	{
		m_positionRotation = Salle::Orientation::Gauche;

		m_textureSalle = loader.obtenirTexture("images/pile radio isotopique/gauche.png");
		m_textureConnexions = loader.obtenirTexture("images/pile radio isotopique/gauche.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;
		m_x = -m_w / 2;
		m_y = -m_h / 2;

		// Connexions

		Connexion cGauche;

		cGauche.setCoord(0, (m_h - 10) / 2, 16, 10);
		cGauche.setDirection(Connexion::Direction::Gauche);
		cGauche.setType(Connexion::Type::PanneauSolaire);
		m_connexions[0] = cGauche;
	}
	else
	{
		m_positionRotation = Salle::Orientation::Haut;

		m_textureSalle = loader.obtenirTexture("images/pile radio isotopique/haut.png");
		m_textureConnexions = loader.obtenirTexture("images/pile radio isotopique/haut.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;
		m_x = -m_w / 2;
		m_y = -m_h / 2;

		// Connexions

		Connexion cHaut;

		cHaut.setCoord((m_w - 10) / 2, 0, 10, 16);
		cHaut.setDirection(Connexion::Direction::Haut);
		cHaut.setType(Connexion::Type::PanneauSolaire);
		m_connexions[0] = cHaut;
	}
}


// FONCTIONS MEMBRES DE LA CLASSE RESERVE_EAU


ReserveEau::ReserveEau(Loader& loader)
{
	m_energieConso = 2;

	m_positionRotation = Salle::Orientation::Verticale;

	m_textureSalle = loader.obtenirTexture("images/reserve eau/verticale/salle.png");
	m_textureConnexions = loader.obtenirTexture("images/reserve eau/verticale/connexions.png");

	m_w = (m_textureSalle->getSize()).x;
	m_h = (m_textureSalle->getSize()).y;
	m_x = -m_w / 2;
	m_y = -m_h / 2;

	// Noeuds

	Noeud* noeudMilieu(new Noeud), *noeudMachine(new Noeud), *noeudConnexionHaut(new Noeud), *noeudConnexionBas(new Noeud);

	noeudMilieu->setCoord(m_w/2, m_h/2);
	noeudMilieu->setType(Noeud::Type::Normal);

	noeudMachine->setCoord(m_w/2, m_h/2);
	noeudMachine->connecter(noeudMilieu);
	noeudMachine->setType(Noeud::Type::Machine);

	noeudConnexionHaut->setCoord(m_w/2, 0);
	noeudConnexionHaut->connecter(noeudMilieu);
	noeudConnexionHaut->setType(Noeud::Type::Connexion);

	noeudConnexionBas->setCoord(m_w/2, m_h);
	noeudConnexionBas->connecter(noeudMilieu);
	noeudConnexionBas->setType(Noeud::Type::Connexion);

	m_noeuds.push_back(noeudMilieu);
	m_noeuds.push_back(noeudMachine);
	m_noeuds.push_back(noeudConnexionHaut);
	m_noeuds.push_back(noeudConnexionBas);

	// Connexions

	Connexion cBas, cHaut;

	cBas.setCoord((m_w - 76)/2, m_h - 16, 76, 16);
	cBas.setDirection(Connexion::Direction::Bas);
	cBas.setNoeud(noeudConnexionBas);
	m_connexions.push_back(cBas);

	cHaut.setCoord((m_w - 76)/2, 0, 76, 16);
	cHaut.setDirection(Connexion::Direction::Haut);
	cHaut.setNoeud(noeudConnexionHaut);
	m_connexions.push_back(cHaut);

	// Terminal radiateur

	m_machines.push_back(new ReservoirEau(loader));
	m_machines[0]->setNoeudProche(noeudMachine);
}


void ReserveEau::effectuerRotation(Loader& loader)
{
	if (m_positionRotation == Salle::Orientation::Verticale)
	{
		m_positionRotation = Salle::Orientation::Horizontale;

		m_textureSalle = loader.obtenirTexture("images/reserve eau/horizontale/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/reserve eau/horizontale/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;

		// Noeuds

		m_noeuds[0]->setCoord(m_w/2, m_h/2);
		m_noeuds[1]->setCoord(m_w/2, m_h/2);

		m_noeuds[2]->setCoord(0, m_h/2);
		m_noeuds[3]->setCoord(m_w, m_h/2);

		// Connexions

		Connexion cGauche, cDroite;

		cGauche.setCoord(0, (m_h - 76)/2, 16, 76);
		cGauche.setDirection(Connexion::Direction::Gauche);
		cGauche.setNoeud(m_noeuds[2]);
		m_connexions[0] = cGauche;

		cDroite.setCoord(m_w - 16, (m_h - 76)/2, 16, 76);
		cDroite.setDirection(Connexion::Direction::Droite);
		cDroite.setNoeud(m_noeuds[3]);
		m_connexions[1] = cDroite;
	}
	else
	{
		m_positionRotation = Salle::Orientation::Verticale;

		m_textureSalle = loader.obtenirTexture("images/reserve eau/verticale/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/reserve eau/verticale/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;

		// Noeuds

		m_noeuds[0]->setCoord(m_w/2, m_h/2);
		m_noeuds[1]->setCoord(m_w/2, m_h/2);

		m_noeuds[2]->setCoord(m_w/2, m_h);
		m_noeuds[3]->setCoord(m_w/2, 0);

		// Connexions

		Connexion cBas, cHaut;

		cBas.setCoord((m_w - 76)/2, m_h - 16, 76, 16);
		cBas.setDirection(Connexion::Direction::Bas);
		cBas.setNoeud(m_noeuds[2]);
		m_connexions[0] = cBas;

		cHaut.setCoord((m_w - 76)/2, 0, 76, 16);
		cHaut.setDirection(Connexion::Direction::Haut);
		cHaut.setNoeud(m_noeuds[3]);
		m_connexions[1] = cHaut;
	}

	m_machines[0]->effectuerRotation(loader);
}


// FONCTIONS MEMBRES DE LA CLASSE SALLE_DE_REFROIDISSEMENT


SalleDeRefroidissement::SalleDeRefroidissement(Loader& loader)
{
	m_energieConso = 2;
	m_chaleurDissipee = 10;

	m_positionRotation = Salle::Orientation::Verticale;

	m_textureSalle = loader.obtenirTexture("images/salle de refroidissement/verticale/salle.png");
	m_textureConnexions = loader.obtenirTexture("images/salle de refroidissement/verticale/connexions.png");

	m_w = (m_textureSalle->getSize()).x;
	m_h = (m_textureSalle->getSize()).y;
	m_x = -m_w / 2;
	m_y = -m_h / 2;

	// Noeuds

	Noeud *noeudMilieu(new Noeud), *noeudMachine(new Noeud), *noeudConnexionHaut(new Noeud), *noeudConnexionBas(new Noeud);

	noeudMilieu->setCoord(m_w/2, m_h/2);
	noeudMilieu->setType(Noeud::Type::Normal);
	
	noeudMachine->setCoord(m_w/2, m_h/2);
	noeudMachine->connecter(noeudMilieu);
	noeudMachine->setType(Noeud::Type::Machine);

	noeudConnexionHaut->setCoord(m_w/2, 0);
	noeudConnexionHaut->connecter(noeudMilieu);
	noeudConnexionHaut->setType(Noeud::Type::Connexion);

	noeudConnexionBas->setCoord(m_w/2, m_h);
	noeudConnexionBas->connecter(noeudMilieu);
	noeudConnexionBas->setType(Noeud::Type::Connexion);

	m_noeuds.push_back(noeudMilieu);
	m_noeuds.push_back(noeudMachine);
	m_noeuds.push_back(noeudConnexionHaut);
	m_noeuds.push_back(noeudConnexionBas);

	// Connexions

	Connexion cBas, cHaut, cGauche, cDroite;

	cBas.setCoord((m_w - 76)/2, m_h - 16, 76, 16);
	cBas.setDirection(Connexion::Direction::Bas);
	cBas.setNoeud(noeudConnexionBas);
	m_connexions.push_back(cBas);

	cHaut.setCoord((m_w - 76)/2, 0, 76, 16);
	cHaut.setDirection(Connexion::Direction::Haut);
	cHaut.setNoeud(noeudConnexionHaut);
	m_connexions.push_back(cHaut);

	cGauche.setCoord(0, (m_h - 10) / 2, 45, 10);
	cGauche.setDirection(Connexion::Direction::Gauche);
	cGauche.setType(Connexion::Type::PanneauRadiateur);
	m_connexions.push_back(cGauche);

	cDroite.setCoord(m_w - 36, (m_h - 10) / 2, 36, 10);
	cDroite.setDirection(Connexion::Direction::Droite);
	cDroite.setType(Connexion::Type::PanneauRadiateur);
	m_connexions.push_back(cDroite);

	// Terminal radiateur

	m_machines.push_back(new TerminalRadiateur(loader));
	m_machines[0]->setNoeudProche(noeudMachine);
}


void SalleDeRefroidissement::effectuerRotation(Loader& loader)
{
	if (m_positionRotation == Salle::Orientation::Verticale)
	{
		m_positionRotation = Salle::Orientation::Horizontale;

		m_textureSalle = loader.obtenirTexture("images/salle de refroidissement/horizontale/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/salle de refroidissement/horizontale/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;

		// Noeuds

		m_noeuds[0]->setCoord(m_w/2, m_h/2);
		m_noeuds[1]->setCoord(m_w/2, m_h/2);

		m_noeuds[2]->setCoord(0, m_h/2);
		m_noeuds[3]->setCoord(m_w, m_h/2);

		// Connexions

		Connexion cGauche, cDroite, cHaut, cBas;

		cGauche.setCoord(0, (m_h - 76)/2, 16, 76);
		cGauche.setDirection(Connexion::Direction::Gauche);
		cGauche.setNoeud(m_noeuds[2]);
		m_connexions[0] = cGauche;

		cDroite.setCoord(m_w - 16, (m_h - 76)/2, 16, 76);
		cDroite.setDirection(Connexion::Direction::Droite);
		cDroite.setNoeud(m_noeuds[3]);
		m_connexions[1] = cDroite;

		cHaut.setCoord((m_w - 10)/2, 0, 10, 36);
		cHaut.setDirection(Connexion::Direction::Haut);
		cHaut.setType(Connexion::Type::PanneauRadiateur);
		m_connexions[2] = cHaut;

		cBas.setCoord((m_w - 10)/2, m_h - 45, 10, 45);
		cBas.setDirection(Connexion::Direction::Bas);
		cBas.setType(Connexion::Type::PanneauRadiateur);
		m_connexions[3] = cBas;
	}
	else
	{
		m_positionRotation = Salle::Orientation::Verticale;

		m_textureSalle = loader.obtenirTexture("images/salle de refroidissement/verticale/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/salle de refroidissement/verticale/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;

		// Noeuds

		m_noeuds[0]->setCoord(m_w/2, m_h/2);
		m_noeuds[1]->setCoord(m_w/2, m_h/2);

		m_noeuds[2]->setCoord(m_w/2, m_h);
		m_noeuds[3]->setCoord(m_w/2, 0);

		// Connexions

		Connexion cBas, cHaut, cGauche, cDroite;

		cBas.setCoord((m_w - 76)/2, m_h - 16, 76, 16);
		cBas.setDirection(Connexion::Direction::Bas);
		cBas.setNoeud(m_noeuds[2]);
		m_connexions[0] = cBas;

		cHaut.setCoord((m_w - 76)/2, 0, 76, 16);
		cHaut.setDirection(Connexion::Direction::Haut);
		cHaut.setNoeud(m_noeuds[3]);
		m_connexions[1] = cHaut;

		cGauche.setCoord(0, (m_h - 10) / 2, 45, 10);
		cGauche.setDirection(Connexion::Direction::Gauche);
		cGauche.setType(Connexion::Type::PanneauRadiateur);
		m_connexions[2] = cGauche;

		cDroite.setCoord(m_w - 36, (m_h - 10) / 2, 36, 10);
		cDroite.setDirection(Connexion::Direction::Droite);
		cDroite.setType(Connexion::Type::PanneauRadiateur);
		m_connexions[3] = cDroite;
	}

	m_machines[0]->effectuerRotation(loader);
}


// FONCTIONS MEMBRES DE LA CLASSE PANNEAU_RADIATEUR


PanneauRadiateur::PanneauRadiateur(Loader& loader)
{
	m_impasse = true;

	m_energieConso = 2;
	m_chaleurDissipee = 10;

	m_positionRotation = Salle::Orientation::Horizontale;

	m_textureSalle = loader.obtenirTexture("images/panneau radiateur/horizontal/salle.png");
	m_textureConnexions = loader.obtenirTexture("images/panneau radiateur/horizontal/connexions.png");

	m_w = (m_textureSalle->getSize()).x;
	m_h = (m_textureSalle->getSize()).y;
	m_x = -m_w / 2;
	m_y = -m_h / 2;

	Connexion cGauche, cDroite;

	cGauche.setCoord(0, (m_h - 10) / 2, 16, 10);
	cGauche.setDirection(Connexion::Direction::Gauche);
	cGauche.setType(Connexion::Type::PanneauRadiateur);
	m_connexions.push_back(cGauche);

	cDroite.setCoord(m_w - 16, (m_h - 10) / 2, 16, 10);
	cDroite.setDirection(Connexion::Direction::Droite);
	cDroite.setType(Connexion::Type::PanneauRadiateur);
	m_connexions.push_back(cDroite);
}


void PanneauRadiateur::effectuerRotation(Loader& loader)
{
	if (m_positionRotation == Salle::Orientation::Verticale)
	{
		m_positionRotation = Salle::Orientation::Horizontale;

		m_textureSalle = loader.obtenirTexture("images/panneau radiateur/horizontal/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/panneau radiateur/horizontal/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;

		Connexion cGauche, cDroite;

		cGauche.setCoord(0, (m_h - 10) / 2, 16, 10);
		cGauche.setDirection(Connexion::Direction::Gauche);
		cGauche.setType(Connexion::Type::PanneauRadiateur);
		m_connexions[0] = cGauche;

		cDroite.setCoord(m_w - 16, (m_h - 10) / 2, 16, 10);
		cDroite.setDirection(Connexion::Direction::Droite);
		cDroite.setType(Connexion::Type::PanneauRadiateur);
		m_connexions[1] = cDroite;
	}
	else
	{
		m_positionRotation = Salle::Orientation::Verticale;

		m_textureSalle = loader.obtenirTexture("images/panneau radiateur/vertical/salle.png");
		m_textureConnexions = loader.obtenirTexture("images/panneau radiateur/vertical/connexions.png");

		m_w = (m_textureSalle->getSize()).x;
		m_h = (m_textureSalle->getSize()).y;

		Connexion cBas, cHaut;

		cBas.setCoord((m_w - 10) / 2, m_h - 16, 10, 16);
		cBas.setDirection(Connexion::Direction::Bas);
		cBas.setType(Connexion::Type::PanneauRadiateur);
		m_connexions[0] = cBas;

		cHaut.setCoord((m_w - 10) / 2, 0, 10, 16);
		cHaut.setDirection(Connexion::Direction::Haut);
		cHaut.setType(Connexion::Type::PanneauRadiateur);
		m_connexions[1] = cHaut;
	}
}
