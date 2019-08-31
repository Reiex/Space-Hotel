#include "Ressources.h"


// FONCTIONS MEMBRES DE LA CLASSE CONTENEUR


Conteneur::Conteneur(int nbEmplacements)
{
	for (int i(0); i < nbEmplacements; i++)
	{
		m_emplacements.push_back(EmplacementRessource(true));
		m_masqueUtilisation.push_back(true);
	}
}


bool Conteneur::ressourceNecessaire(Ressource* ressource, int i) const
{
	return !m_emplacements[i].estOccupe() && m_emplacements[i].estAutorisee(ressource) && m_emplacements[i].ressourceNecessaire();
}


bool Conteneur::ressourceDeposable(Ressource* ressource, int i) const
{
	return (!m_emplacements[i].estOccupe() && m_emplacements[i].estAutorisee(ressource));
}


void Conteneur::deposerRessource(Ressource* ressource, int i)
{
	if (!m_emplacements[i].estOccupe() && m_emplacements[i].estAutorisee(ressource))
	{
		m_emplacements[i].setRessource(ressource);
		ressource->setConteneur(this);
	}
}


int Conteneur::getTaille() const
{
	return m_emplacements.size();
}


bool Conteneur::ressourcePresente(int i) const
{
	return (m_emplacements[i].getRessource() != 0);
}


bool Conteneur::ressourceDisponnible(int i) const
{
	return !m_emplacements[i].ressourceNecessaire();
}


bool Conteneur::ressourceEnTrop(int i) const
{
	return !m_emplacements[i].getEntree() && m_emplacements[i].getRessource() != 0;
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


void Conteneur::reserverEmplacement(int i)
{
	m_masqueUtilisation[i] = false;
}


void Conteneur::libererEmplacement(int i)
{
	m_masqueUtilisation[i] = true;
}


bool Conteneur::emplacementLibre(int i) const
{
	return m_masqueUtilisation[i];
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


bool EmplacementRessource::getEntree() const
{
	return m_entree;
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


int const Ressource::nbTypes(2);
std::string const Ressource::cheminsTypes[Ressource::nbTypes] = {
		"images/interface/logo eau.png",
		"images/interface/logo eau sale.png"
	};


Ressource::Ressource(int type)
{
	m_type = type;
	m_etat = 100;
	m_conteneur = 0;
	m_noeudProche = 0;
}


void Ressource::setType(int type)
{
	m_type = type;
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
	if (m_conteneur != 0)
	{
		m_conteneur->retirerRessource(this);
	}
}


// FONCTIONS MEMBRES DE LA CLASSE MACHINE


Entite const Machine::SurfaceDessinDetails(15, 15, 474, 230);


Machine::Machine(int nbEntrees, int nbSorties) : Conteneur(nbEntrees + nbSorties)
{
	m_noeudProche = 0;

	for (int i(nbEntrees); i < nbEntrees + nbSorties; i++)
	{
		m_emplacements[i].setEntree(false);
	}

	m_energieConso = 0;
	m_chaleurDissipee = 0;
	m_texture = 0;
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


// FONCTIONS MEMBRES DE LA CLASSE CONTENEUR_EAU


ReservoirEau::ReservoirEau(Loader& loader) : Machine(20, 0)
{
	for (int i(0); i < Ressource::nbTypes; i++)
	{
		if (i != Ressource::Eau && i != Ressource::EauSale)
		{
			for (int j(0); j < 20; j++)
			{
				m_emplacements[j].interdireRessource(i);
			}
		}

		if (i == Ressource::Eau)
		{
			for (int j(10); j < 20; j++)
			{
				m_emplacements[j].interdireRessource(i);
			}
		}

		if (i == Ressource::EauSale)
		{
			for (int j(0); j < 10; j++)
			{
				m_emplacements[j].interdireRessource(i);
			}
		}
	}

	m_x = 21;
	m_y = 55;
	m_w = 132;
	m_h = 97;

	m_texture = loader.obtenirTexture("images/reserve eau/verticale/reservoirs.png");

	m_energieConso = 1;

	m_renderTexture.create(m_w, m_h);
	m_remplissage[0] = -1;
	m_remplissage[1] = 0;
	m_vertical = true;
}


void ReservoirEau::faireFonctionner(float dt, bool panneElectrique)
{

}


void ReservoirEau::effectuerRotation(Loader& loader)
{
	m_vertical = !m_vertical;
	m_remplissage[0] = -1;
	if (m_texture == loader.obtenirTexture("images/reserve eau/verticale/reservoirs.png"))
	{
		m_x = 55;
		m_y = 27;
		m_w = 97;
		m_h = 132;

		m_texture = loader.obtenirTexture("images/reserve eau/horizontale/reservoirs.png");
	}
	else
	{
		m_x = 21;
		m_y = 55;
		m_w = 132;
		m_h = 97;

		m_texture = loader.obtenirTexture("images/reserve eau/verticale/reservoirs.png");
	}

	m_renderTexture.create(m_w, m_h);
}


sf::Texture* ReservoirEau::getTexture()
{
	// Déterminer si une mise à jour de la texture est nécessaire

	bool majTexture(false);

	int remplissage(0);
	for (int i(0); i < 10; i++)
	{
		if (ressourcePresente(i))
		{
			remplissage++;
		}
	}
	majTexture = (m_remplissage[0] != remplissage);
	m_remplissage[0] = remplissage;

	remplissage = 0;
	for (int i(10); i < 20; i++)
	{
		if (ressourcePresente(i))
		{
			remplissage++;
		}
	}
	majTexture = majTexture || (m_remplissage[1] != remplissage);
	m_remplissage[1] = remplissage;

	// Mettre à jour la texture si nécessaire

	if (majTexture)
	{
		sf::Sprite sprite(*m_texture);
		float facteur[2] = { float(m_remplissage[0]) / 10, float(m_remplissage[1]) / 10 };

		m_renderTexture.clear(sf::Color(0, 0, 0, 0));

		if (m_vertical)
		{
			sprite.setTextureRect(sf::IntRect(47, 15, 38, 65));
			sprite.setPosition(47, 15);
			m_renderTexture.draw(sprite);
			sprite.setTextureRect(sf::IntRect(0, 0, 132, 15));
			sprite.setPosition(0, 0);
			m_renderTexture.draw(sprite);

			sprite.setTextureRect(sf::IntRect(5, 15, 42, 67 * facteur[0]));
			sprite.setPosition(5, 15);
			m_renderTexture.draw(sprite);
			sprite.setTextureRect(sf::IntRect(0, 82, 52, 15));
			sprite.setPosition(0, 15 + 67 * facteur[0]);
			m_renderTexture.draw(sprite);

			sprite.setTextureRect(sf::IntRect(85, 15, 42, 67 * facteur[1]));
			sprite.setPosition(85, 15);
			m_renderTexture.draw(sprite);
			sprite.setTextureRect(sf::IntRect(80, 82, 52, 15));
			sprite.setPosition(80, 15 + 67 * facteur[1]);
			m_renderTexture.draw(sprite);
		}
		else
		{
			sprite.setTextureRect(sf::IntRect(15, 47, 65, 38));
			sprite.setPosition(15, 47);
			m_renderTexture.draw(sprite);
			sprite.setTextureRect(sf::IntRect(0, 0, 15, 132));
			sprite.setPosition(0, 0);
			m_renderTexture.draw(sprite);

			sprite.setTextureRect(sf::IntRect(15, 5, 67 * facteur[1], 42));
			sprite.setPosition(15, 5);
			m_renderTexture.draw(sprite);
			sprite.setTextureRect(sf::IntRect(82, 0, 15, 52));
			sprite.setPosition(15 + 67 * facteur[1], 0);
			m_renderTexture.draw(sprite);

			sprite.setTextureRect(sf::IntRect(15, 85, 67 * facteur[0], 42));
			sprite.setPosition(15, 85);
			m_renderTexture.draw(sprite);
			sprite.setTextureRect(sf::IntRect(82, 80, 15, 52));
			sprite.setPosition(15 + 67 * facteur[0], 80);
			m_renderTexture.draw(sprite);
		}

		m_renderTexture.display();
		m_textureFinale = m_renderTexture.getTexture();
	}

	return &m_textureFinale;
}


void ReservoirEau::afficherDetails(sf::RenderWindow& window, Loader& loader, bool panneElectrique) const
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

		Réserve d'eau:

			[] [] [] [] [] [] [] [] [] []

		Réserve d'eau sale:

			[] [] [] [] [] [] [] [] [] []
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

	// Réserve d'eau

	int nbEau(0);
	texte.setString(sf::String("Réserve d'eau:"));
	texte.setPosition(x, y + 35 + (h - 35 - 140)/3);
	window.draw(texte);

	for (int i(0); i < 10; i++)
	{
		nbEau += m_emplacements[i].estOccupe();
	}

	texture = loader.obtenirTexture("images/interface/logo eau.png");
	sprite.setTexture(*texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
	sprite.setScale(0.3, 0.3);
	for (int i(0); i < nbEau; i++)
	{
		sprite.setPosition(x + (w - 390)/2 + 40*i, y + 35 + (h - 35 - 140)/3 + 40);
		window.draw(sprite);
	}

	texture = loader.obtenirTexture("images/interface/logo ressource manquante.png");
	sprite.setTexture(*texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
	sprite.setScale(0.3, 0.3);
	for (int i(nbEau); i < 10; i++)
	{
		sprite.setPosition(x + (w - 390)/2 + 40*i, y + 35 + (h - 35 - 140)/3 + 40);
		window.draw(sprite);
	}

	// Réserve d'eau sale

	nbEau = 0;
	texte.setString(sf::String("Réserve d'eau sale:"));
	texte.setPosition(x, y + 35 + 2*(h - 35 - 140)/3 + 70);
	window.draw(texte);

	for (int i(10); i < 20; i++)
	{
		nbEau += m_emplacements[i].estOccupe();
	}

	texture = loader.obtenirTexture("images/interface/logo eau sale.png");
	sprite.setTexture(*texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
	sprite.setScale(0.3, 0.3);
	for (int i(0); i < nbEau; i++)
	{
		sprite.setPosition(x + (w - 390)/2 + 40*i, y + 35 + 2*(h - 35 - 140)/3 + 70 + 40);
		window.draw(sprite);
	}

	texture = loader.obtenirTexture("images/interface/logo ressource manquante.png");
	sprite.setTexture(*texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
	sprite.setScale(0.3, 0.3);
	for (int i(nbEau); i < 10; i++)
	{
		sprite.setPosition(x + (w - 390)/2 + 40*i, y + 35 + 2*(h - 35 - 140)/3 + 70 + 40);
		window.draw(sprite);
	}
}


// FONCTIONS MEMBRES DE LA CLASSE FILTRE_EAU


FiltreEau::FiltreEau(Loader& loader) : Machine(1, 1)
{
	m_emplacements[0].setRessourceNecessaire(true);
	for (int i(0); i < Ressource::nbTypes; i++)
	{
		if (i != Ressource::EauSale)
		{
			m_emplacements[0].interdireRessource(i);
		}
		if (i != Ressource::Eau)
		{
			m_emplacements[1].interdireRessource(i);
		}
	}

	m_x = 27;
	m_y = 33;
	m_w = 64;
	m_h = 141;

	m_texture = loader.obtenirTexture("images/salle de traitement des eaux usees/verticale/filtre.png");

	m_energieConso = 5;

	m_avancement = 0;
}


void FiltreEau::faireFonctionner(float dt, bool panneElectrique)
{
	if (panneElectrique || m_emplacements[0].getRessource() == 0 || m_emplacements[1].getRessource() != 0)
	{
		m_avancement = 0;
	}
	else
	{
		m_avancement += dt / 30;
		if (m_avancement >= 1)
		{
			Ressource* ressource(m_emplacements[0].getRessource());
			m_emplacements[0].setRessource(0);
			ressource->setType(Ressource::Eau);
			m_emplacements[1].setRessource(ressource);
		}
	}
}


void FiltreEau::effectuerRotation(Loader& loader)
{
	if (m_texture == loader.obtenirTexture("images/salle de traitement des eaux usees/verticale/filtre.png"))
	{
		m_x = 33;
		m_y = 43;
		m_w = 64;
		m_h = 141;

		m_texture = loader.obtenirTexture("images/salle de traitement des eaux usees/horizontale/filtre.png");
	}
	else
	{
		m_x = 27;
		m_y = 33;
		m_w = 64;
		m_h = 141;

		m_texture = loader.obtenirTexture("images/salle de traitement des eaux usees/verticale/filtre.png");
	}
}


void FiltreEau::afficherDetails(sf::RenderWindow& window, Loader& loader, bool panneElectrique) const
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

		Filtrage de l'eau:

			[] ===> []

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

	// Avancement du filtrage de l'eau

	texte.setString(sf::String("Filtrage de l'eau:"));
	texte.setPosition(x, y + 40 + (h - 80)/2);
	window.draw(texte);

	if (m_emplacements[0].estOccupe())
	{
		texture = loader.obtenirTexture("images/interface/logo eau sale.png");
	}
	else
	{
		texture = loader.obtenirTexture("images/interface/logo ressource manquante.png");
	}
	sprite.setTexture(*texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
	sprite.setPosition(x + (w - 160)/2, y + 40 + (h - 80)/2 + 40);
	sprite.setScale(0.3, 0.3);
	window.draw(sprite);

	if (m_emplacements[1].getRessource() != 0)
	{
		texture = loader.obtenirTexture("images/interface/logo eau.png");
	}
	else
	{
		texture = loader.obtenirTexture("images/interface/logo ressource manquante.png");
	}
	sprite.setTexture(*texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
	sprite.setPosition(x + (w - 160)/2 + 130, y + 40 + (h - 80)/2 + 40);
	sprite.setScale(0.3, 0.3);
	window.draw(sprite);

	texture = loader.obtenirTexture("images/interface/detail machine/progression.png");
	sprite.setTexture(*texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 84, 30));
	sprite.setPosition(x + (w - 160)/2 + 48, y + 40 + (h - 80)/2 + 40);
	sprite.setScale(1, 1);
	window.draw(sprite);

	sprite.setTextureRect(sf::IntRect(0, 30, 75*m_avancement, 22));
	sprite.setPosition(x + (w - 160)/2 + 51, y + 40 + (h - 80)/2 + 44);
	window.draw(sprite);
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


void TerminalRadiateur::faireFonctionner(float dt, bool panneElectrique)
{
	m_chaleurDissipee = 0;

	if (!panneElectrique)
	{
		m_chaleurDissipee = (ressourcePresente(0) + ressourcePresente(1)) * 20;
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


void TerminalRadiateur::afficherDetails(sf::RenderWindow& window, Loader& loader, bool panneElectrique) const
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
	texte.setPosition(x, y + h / 2 - 50);
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
	sprite.setPosition(x + (w - 200) / 2, y + h / 2 + 20);
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
	sprite.setPosition(x + (w - 200) / 2 + 40, y + h / 2 + 20);
	sprite.setScale(0.3, 0.3);
	window.draw(sprite);

	texture = loader.obtenirTexture("images/interface/detail machine/progression.png");
	sprite.setTexture(*texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 84, 30));
	sprite.setPosition(x + (w - 200) / 2 + 78, y + h / 2 + 20);
	sprite.setScale(1, 1);
	window.draw(sprite);

	sprite.setTextureRect(sf::IntRect(0, 30, 37.5 * nbEau, 22));
	sprite.setPosition(x + (w - 200) / 2 + 81, y + h / 2 + 24);
	window.draw(sprite);

	texture = loader.obtenirTexture("images/interface/logo temperature.png");
	sprite.setTexture(*texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
	sprite.setPosition(x + (w - 200) / 2 + 170, y + h / 2 + 20);
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

	if (panneElectrique)
	{
		texte.setString(sf::String(float_to_string(m_chaleurDissipee)) + "(" + std::to_string(nbEau * 20) + ")");
	}
	else
	{
		texte.setString(sf::String(float_to_string(m_chaleurDissipee)));
	}
	texte.setPosition(x + wTexte + 50, y + h - 40);
	texte.setCharacterSize(30);
	window.draw(texte);
}
