#include "Menu.h"


// FONCTIONS MEMBRES DE LA CLASSE FENETRE_DESCRIPTION


FenetreDescription::FenetreDescription(Loader& loader, sf::VideoMode const& resolution, std::string const& chaine)
{
	m_texture = creerTextureFenetre(loader, 20, chaine);

	float scaleX(float(resolution.width) / 1920.0), scaleY(float(resolution.height) / 1080.0);

	setCoord(0, 0, m_texture.getSize().x * scaleX, m_texture.getSize().y * scaleY);
}


void FenetreDescription::afficher(sf::RenderWindow& window) const
{
	if (estActive(window))
	{
		sf::Sprite sprite(m_texture);
		sprite.setPosition(m_xSprite, m_ySprite);
		window.draw(sprite);
	}
}


void FenetreDescription::setPosition(int x, int y)
{
	float scaleX(getW() / m_texture.getSize().x), scaleY(getH() / m_texture.getSize().y);

	m_xSprite = x;
	m_ySprite = y;
	setCoord(x * scaleX, y * scaleY);
}


void FenetreDescription::setDeclencheur(Entite* entite)
{
	m_declencheur = entite;
}


bool FenetreDescription::estActive(sf::RenderWindow const& window) const
{
	return m_declencheur->collisionSouris(window);
}


// FONCTIONS MEMBRES DE LA CLASSE FENETRE_INFORMATION


FenetreInformation::FenetreInformation(Loader& loader, sf::VideoMode const& resolution, std::string const& chaine)
{
	m_texture = creerTextureFenetre(loader, 30, chaine);

	float scaleX(float(resolution.width) / 1920.0), scaleY(float(resolution.height) / 1080.0);

	m_tempsAffichage = 5;
	m_xSprite = (1920 - m_texture.getSize().x) / 2;
	m_ySprite = 10;
	setCoord(m_xSprite * scaleX, m_ySprite * scaleY, m_texture.getSize().x * scaleX, m_texture.getSize().y * scaleY);
}


void FenetreInformation::afficher(sf::RenderWindow& window)
{
	float ratio(m_tempsAffiche.getElapsedTime().asSeconds() / m_tempsAffichage);
	float coeff(exp(-pow(ratio * 1.5, 4)));

	sf::Sprite sprite(m_texture);
	sprite.setPosition(m_xSprite, m_ySprite);
	sprite.setColor(sf::Color(255, 255, 255, 255 * coeff));
	window.draw(sprite);
}


void FenetreInformation::ajusterDecalage()
{
	float scaleY(getY() / m_ySprite);

	m_ySprite = 50;
	setCoord(getX(), m_ySprite * scaleY);
}


void FenetreInformation::ajusterDecalage(FenetreInformation& fenetre)
{
	float scaleY(getY() / m_ySprite);

	m_ySprite = fenetre.m_ySprite + fenetre.m_texture.getSize().y + 10;
	setCoord(getX(), fenetre.getY() + fenetre.getH() + 10 * scaleY);
}


bool FenetreInformation::estActive(sf::RenderWindow const& window)
{
	if (collisionSouris(window))
	{
		m_tempsAffiche.restart();
		return true;
	}
	else
	{
		return (m_tempsAffiche.getElapsedTime().asSeconds() < m_tempsAffichage);
	}
}


// FONCTIONS MEMBRES DE LA CLASSE BOUTON


void Bouton::afficher(sf::RenderWindow& window) const
{
	if (collisionSouris(window))
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			window.draw(m_spriteOnclick);
		}
		else
		{
			window.draw(m_spriteHover);
		}
	}
	else
	{
		window.draw(m_sprite);
	}
}


void Bouton::afficherDescription(sf::RenderWindow& window) const
{
	if (m_description != 0)
	{
		m_description->afficher(window);
	}
}


void Bouton::setDescription(FenetreDescription* description)
{
	m_description = description;
}


void Bouton::setSprite(sf::Sprite& sprite, sf::Sprite& spriteHover, sf::Sprite& spriteOnclick)
{
	m_sprite = sprite;
	m_spriteHover = spriteHover;
	m_spriteOnclick = spriteOnclick;
}


// FONCTIONS MEMBRES DE LA CLASSE MENU


void Menu::afficher(sf::RenderWindow& window) const
{
	window.draw(m_spriteFond);

	for (int i(0); i < m_boutons.size(); i++)
	{
		m_boutons[i].afficher(window);
	}
}


void Menu::afficherDescriptions(sf::RenderWindow& window) const
{
	for (int i(0); i < m_boutons.size(); i++)
	{
		m_boutons[i].afficherDescription(window);
	}

	for (int i(0); i < m_descriptions.size(); i++)
	{
		m_descriptions[i].afficher(window);
	}
}


int Menu::getClicBouton(sf::RenderWindow const& window) const
{
	if (!m_afficher)
	{
		return -1;
	}

	for (int i(0); i < m_boutons.size(); i++)
	{
		if (m_boutons[i].collisionSouris(window))
		{
			return i;
		}
	}

	return -1;
}


void Menu::setAfficher(bool afficher)
{
	m_afficher = afficher;
}


bool Menu::estAffiche() const
{
	return m_afficher;
}


// FONCTIONS MEMBRES DE LA CLASSE HUD_GAUCHE


HudGauche::HudGauche(Loader& loader, sf::VideoMode const& resolution)
{
	m_afficher = true;

	float scaleX(float(resolution.width) / 1920.0), scaleY(float(resolution.height) / 1080.0);

	setCoord(0, 0, 125 * scaleX, 1080 * scaleY);
	m_spriteFond = sf::Sprite(*loader.obtenirTexture("images/interface/HUD gauche/fond.png"), sf::IntRect(0, 0, 125, 1080));
	m_spriteFond.setPosition(0, 0);

	sf::IntRect coordBoutons[2] = {
		sf::IntRect(10, 10, 100, 100),  // Menu options
		sf::IntRect(10, 120, 100, 100)  // Creation de salle
	};

	std::string chainesDescriptions[2] = {
		"Menu principal",
		"Cr�er une nouvelle salle"
	};

	for (int i(0); i < 2; i++)
	{
		sf::Sprite sprite(*loader.obtenirTexture("images/interface/HUD gauche/boutons.png"), coordBoutons[i]);
		sf::Sprite spriteHover(*loader.obtenirTexture("images/interface/HUD gauche/boutons hover.png"), coordBoutons[i]);
		sf::Sprite spriteOnclick(*loader.obtenirTexture("images/interface/HUD gauche/boutons onclick.png"), coordBoutons[i]);

		sprite.setPosition(coordBoutons[i].left, coordBoutons[i].top);
		spriteHover.setPosition(coordBoutons[i].left, coordBoutons[i].top);
		spriteOnclick.setPosition(coordBoutons[i].left, coordBoutons[i].top);

		Bouton bouton;
		bouton.setCoord(coordBoutons[i].left * scaleX, coordBoutons[i].top * scaleY, coordBoutons[i].width * scaleX, coordBoutons[i].height * scaleY);
		bouton.setSprite(sprite, spriteHover, spriteOnclick);

		FenetreDescription* description(new FenetreDescription(loader, resolution, chainesDescriptions[i]));
		description->setPosition(coordBoutons[i].left + coordBoutons[i].width + 5, coordBoutons[i].top + 20);
		Entite* declencheur(new Entite(coordBoutons[i].left * scaleX, coordBoutons[i].top * scaleY, coordBoutons[i].width * scaleX, coordBoutons[i].height * scaleY));
		description->setDeclencheur(declencheur);

		bouton.setDescription(description);

		m_boutons.push_back(bouton);
	}
}


// FONCTIONS MEMBRES DE LA CLASSE HUD_DROIT


HudDroit::HudDroit(Loader& loader, sf::VideoMode const& resolution)
{
	m_afficher = true;

	float scaleX(float(resolution.width) / 1920.0), scaleY(float(resolution.height) / 1080.0);

	setCoord(1765, 0, 155 * scaleX, 1080 * scaleY);
	m_spriteFond = sf::Sprite(*loader.obtenirTexture("images/interface/HUD droit/fond.png"), sf::IntRect(1765, 0, 155, 1080));
	m_spriteFond.setPosition(1765, 0);

	// Ajouter les descriptions des barres de ressources

	int const nbDeclencheurs(5);

	sf::IntRect coordDeclencheurs[nbDeclencheurs] = {
		sf::IntRect(1780, 925, 130, 30),
		sf::IntRect(1780, 885, 130, 30),
		sf::IntRect(1780, 845, 130, 30),
		sf::IntRect(1780, 805, 130, 30),
		sf::IntRect(1780, 765, 130, 30)
	};

	std::string chainesDeclenchees[nbDeclencheurs] = {
		"Avanc�e du temps\nLa nuit correspond � un passage de la station spatiale\nderri�re la planete autour de laquelle elle gravite.\nLes panneaux solaire ne peuvent alors plus fonctionner.",
		"Pourcentage d'�lectricit� consomm�e\nsur l'�lectricit� totale produite.",
		"Pourcentage d'�lectricit� stock�e\nsur la capacit� totale des accumulateurs.",
		"Nombre d'habitants de la station.",
		"Temp�rature de la station.\nLa temp�rature normale est de 15�C."
	};

	for (int i(0); i < nbDeclencheurs; i++)
	{
		FenetreDescription description(loader, resolution, chainesDeclenchees[i]);
		description.setPosition(coordDeclencheurs[i].left - description.getW() - 5, coordDeclencheurs[i].top + coordDeclencheurs[i].height - description.getH());
		Entite* declencheur(new Entite(coordDeclencheurs[i].left * scaleX, coordDeclencheurs[i].top * scaleY, coordDeclencheurs[i].width * scaleX, coordDeclencheurs[i].height * scaleY));
		description.setDeclencheur(declencheur);

		m_descriptions.push_back(description);
	}
}


// FONCTIONS MEMBRES DE LA CLASSE MENU_CREATION_SALLE


MenuCreationSalle::MenuCreationSalle(Loader& loader, sf::VideoMode const& resolution)
{
	m_afficher = false;

	float scaleX(resolution.width / 1920), scaleY(resolution.height / 1080);

	setCoord(130 * scaleX, 104 * scaleY, 460 * scaleX, 460 * scaleY);
	m_spriteFond = sf::Sprite(*loader.obtenirTexture("images/interface/creation salle/fond.png"), sf::IntRect(130, 104, 460, 460));
	m_spriteFond.setPosition(130 * scaleX, 104 * scaleY);

	sf::IntRect coordBoutons[12] = {

		sf::IntRect(145, 119, 100, 100),  // Couloir I
		sf::IntRect(255, 119, 100, 100),  // Couloir T
		sf::IntRect(365, 119, 100, 100),  // Couloir L
		sf::IntRect(475, 119, 100, 100),  // Couloir X

		sf::IntRect(145, 229, 100, 100),  // Salle de batteries
		sf::IntRect(255, 229, 100, 100),  // Centrale �lectrique
		sf::IntRect(365, 229, 100, 100),  // Panneau solaire
		sf::IntRect(475, 229, 100, 100),  // Pile radio isotopique

		sf::IntRect(145, 339, 100, 100),  // R�serve d'eau
		sf::IntRect(255, 339, 100, 100),  // Salle de traitement des eaux us�es

		sf::IntRect(145, 449, 100, 100),  // Salle de refroidissement
		sf::IntRect(255, 449, 100, 100)  // Panneau radiateur
	};

	std::string chainesDescriptions[12] = {
		"Couloir I\nSalle vide.",
		"Couloir T\nSalle vide.",
		"Couloir L\nSalle vide.",
		"Couloir X\nSalle vide.",

		"Salle des batteries\nSalle munie d'accumulateurs permettant de stocker l'�lectricit� produite en surplus.",
		"Centrale �lectrique\nSalle munie d'un transformateur �lectrique et de panneaux solaires.\nPermet de produire l'�lectricit� qui sera ensuite consomm�e par les autres salles.",
		"Panneau solaire\nPeut �tre attach� � une centrale �lectrique.\nProduit de l'�lectricit� tant que le soleil l'�claire.",
		"Pile radioisotopique\nPeut �tre attach� � une centrale �lectrique.\nProduit de l'�lectricit� en continu, mais � un rendement moins �lev� que les panneaux solaires.",
		
		"R�serve d'eau\nSalle permettant de stocker s�par�ment de l'eau propre et de l'eau us�e.",
		"Salle de traitement des eaux us�es\nSalle permettant de transformer l'eau us�e en eau propre.",

		"Salle de refroidissement\nPermet � la station de ne pas surchauffer.",
		"Panneau radiateur\nPeut �tre attach� � une salle de refroidissement pour augmenter son efficacit�."
	};

	for (int i(0); i < 12; i++)
	{
		sf::Sprite sprite(*loader.obtenirTexture("images/interface/creation salle/boutons.png"), coordBoutons[i]);
		sf::Sprite spriteHover(*loader.obtenirTexture("images/interface/creation salle/boutons hover.png"), coordBoutons[i]);
		sf::Sprite spriteOnclick(*loader.obtenirTexture("images/interface/creation salle/boutons onclick.png"), coordBoutons[i]);

		sprite.setPosition(coordBoutons[i].left, coordBoutons[i].top);
		spriteHover.setPosition(coordBoutons[i].left, coordBoutons[i].top);
		spriteOnclick.setPosition(coordBoutons[i].left, coordBoutons[i].top);

		Bouton bouton;
		bouton.setCoord(coordBoutons[i].left * scaleX, coordBoutons[i].top * scaleY, coordBoutons[i].width * scaleX, coordBoutons[i].height * scaleY);
		bouton.setSprite(sprite, spriteHover, spriteOnclick);

		FenetreDescription* description(new FenetreDescription(loader, resolution, chainesDescriptions[i]));
		description->setPosition(coordBoutons[i].left + coordBoutons[i].width + 5, coordBoutons[i].top + 20);
		Entite* declencheur(new Entite(coordBoutons[i].left * scaleX, coordBoutons[i].top * scaleY, coordBoutons[i].width * scaleX, coordBoutons[i].height * scaleY));
		description->setDeclencheur(declencheur);

		bouton.setDescription(description);

		m_boutons.push_back(bouton);
	}
}


// FONCTIONS MEMBRES DE LA CLASSE MENU_DETAIL_SALLE


MenuDetailSalle::MenuDetailSalle(Loader& loader, sf::VideoMode const& resolution)
{
	m_afficher = false;

	float scaleX(float(resolution.width) / 1920.0), scaleY(float(resolution.height) / 1080.0);

	setCoord(0, 0, 155 * scaleX, 1080 * scaleY);
	m_spriteFond = sf::Sprite(*loader.obtenirTexture("images/interface/detail salle/fond.png"), sf::IntRect(0, 0, 155, 1080));
	m_spriteFond.setPosition(0, 0);

	// Ajouter les boutons

	sf::IntRect coordBoutons[1] = {
		sf::IntRect(15, 15, 100, 100)  // Destruction de salle
	};

	std::string chainesDescriptions[1] = {
		"D�truire la ou les salle(s) selectionn�e(s)"
	};

	for (int i(0); i < 1; i++)
	{
		sf::Sprite sprite(*loader.obtenirTexture("images/interface/detail salle/boutons.png"), coordBoutons[i]);
		sf::Sprite spriteHover(*loader.obtenirTexture("images/interface/detail salle/boutons hover.png"), coordBoutons[i]);
		sf::Sprite spriteOnclick(*loader.obtenirTexture("images/interface/detail salle/boutons onclick.png"), coordBoutons[i]);

		sprite.setPosition(coordBoutons[i].left, coordBoutons[i].top);
		spriteHover.setPosition(coordBoutons[i].left, coordBoutons[i].top);
		spriteOnclick.setPosition(coordBoutons[i].left, coordBoutons[i].top);

		Bouton bouton;
		bouton.setCoord(coordBoutons[i].left * scaleX, coordBoutons[i].top * scaleY, coordBoutons[i].width * scaleX, coordBoutons[i].height * scaleY);
		bouton.setSprite(sprite, spriteHover, spriteOnclick);

		FenetreDescription* description(new FenetreDescription(loader, resolution, chainesDescriptions[i]));
		description->setPosition(coordBoutons[i].left + coordBoutons[i].width + 5, coordBoutons[i].top + 20);
		Entite* declencheur(new Entite(coordBoutons[i].left * scaleX, coordBoutons[i].top * scaleY, coordBoutons[i].width * scaleX, coordBoutons[i].height * scaleY));
		description->setDeclencheur(declencheur);

		bouton.setDescription(description);

		m_boutons.push_back(bouton);
	}

	// Ajouter les descriptions des barres de ressources

	sf::IntRect coordDeclencheurs[5] = {
		sf::IntRect(15, 125, 130, 30),
		sf::IntRect(15, 165, 130, 30),
		sf::IntRect(15, 205, 130, 30),
		sf::IntRect(15, 245, 130, 30),
		sf::IntRect(15, 285, 130, 30)
	};

	std::string chainesDeclenchees[5] = {
		"Electricit� produite par\nla salle selectionn�e.",
		"Electricit� consomm�e par\nla salle selectionn�e.",
		"Capacit� des accumulateurs\nde la salle selectionn�e.",
		"Pourcentage d'�nergie stock�e\npar les accumulateurs.",
		"Chaleur dissip�e par\nla salle selectionn�e."
	};

	for (int i(0); i < 5; i++)
	{
		FenetreDescription description(loader, resolution, chainesDeclenchees[i]);
		description.setPosition(coordDeclencheurs[i].left + coordDeclencheurs[i].width - 50, coordDeclencheurs[i].top);
		Entite* declencheur(new Entite(coordDeclencheurs[i].left * scaleX, coordDeclencheurs[i].top * scaleY, coordDeclencheurs[i].width * scaleX, coordDeclencheurs[i].height * scaleY));
		description.setDeclencheur(declencheur);

		m_descriptions.push_back(description);
	}
}


// FONCTIONS MEMBRES DE LA CLASSE MENU_FUSION_SALLES


MenuFusionSalles::MenuFusionSalles(Loader& loader, sf::VideoMode const& resolution)
{
	m_afficher = false;

	float scaleX(float(resolution.width) / 1920.0), scaleY(float(resolution.height) / 1080.0);

	setCoord(0, 828 * scaleY, 155 * scaleX, 252 * scaleY);
	m_spriteFond = sf::Sprite(*loader.obtenirTexture("images/interface/fusion salles/fond.png"), sf::IntRect(0, 828, 155, 252));
	m_spriteFond.setPosition(0, 828 * scaleY);

	// Ajouter le bouton

	sf::IntRect coordBouton(10, 970, 100, 100);
	std::string chaineDescription("Raccorder les deux salles s�lectionn�es.");

	sf::Sprite sprite(*loader.obtenirTexture("images/interface/fusion salles/bouton.png"), coordBouton);
	sf::Sprite spriteHover(*loader.obtenirTexture("images/interface/fusion salles/bouton hover.png"), coordBouton);
	sf::Sprite spriteOnclick(*loader.obtenirTexture("images/interface/fusion salles/bouton onclick.png"), coordBouton);

	sprite.setPosition(coordBouton.left, coordBouton.top);
	spriteHover.setPosition(coordBouton.left, coordBouton.top);
	spriteOnclick.setPosition(coordBouton.left, coordBouton.top);

	Bouton bouton;
	bouton.setCoord(coordBouton.left * scaleX, coordBouton.top * scaleY, coordBouton.width * scaleX, coordBouton.height * scaleY);
	bouton.setSprite(sprite, spriteHover, spriteOnclick);

	FenetreDescription* description(new FenetreDescription(loader, resolution, chaineDescription));
	description->setPosition(coordBouton.left + coordBouton.width + 5, coordBouton.top + 20);
	Entite* declencheur(new Entite(coordBouton.left * scaleX, coordBouton.top * scaleY, coordBouton.width * scaleX, coordBouton.height * scaleY));
	description->setDeclencheur(declencheur);

	bouton.setDescription(description);

	m_boutons.push_back(bouton);
}


// FONCTIONS MEMBRES DE LA CLASSE MENU_FUSION_SALLES


MenuDetailMachine::MenuDetailMachine(Loader& loader, sf::VideoMode const& resolution)
{
	m_afficher = false;

	float scaleX(float(resolution.width) / 1920.0), scaleY(float(resolution.height) / 1080.0);

	setCoord(0, 0, 504 * scaleX, 260 * scaleY);
	m_spriteFond = sf::Sprite(*loader.obtenirTexture("images/interface/detail machine/fond.png"), sf::IntRect(0, 0, 504, 1080));
	m_spriteFond.setPosition(0, 0);
}
