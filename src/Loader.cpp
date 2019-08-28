#include "Loader.h"


Loader::Loader()
{
	// Chargement des images

	int const nombreImages(88);

	std::string cheminsImages[nombreImages] = {
		"images/fond/fond.png",

		"images/interface/fenetre.png",
		"images/interface/logo electricite.png",
		"images/interface/logo capacite.png",
		"images/interface/logo personnage.png",
		"images/interface/logo jour.png",
		"images/interface/logo nuit.png",
		"images/interface/logo eau.png",
		"images/interface/logo temperature.png",
		"images/interface/logo ressource manquante.png",

		"images/interface/HUD gauche/fond.png",
		"images/interface/HUD gauche/boutons.png",
		"images/interface/HUD gauche/boutons hover.png",
		"images/interface/HUD gauche/boutons onclick.png",

		"images/interface/HUD droit/fond.png",
		"images/interface/HUD droit/progression.png",

		"images/interface/creation salle/fond.png",
		"images/interface/creation salle/boutons.png",
		"images/interface/creation salle/boutons hover.png",
		"images/interface/creation salle/boutons onclick.png",

		"images/interface/detail salle/fond.png",
		"images/interface/detail salle/boutons.png",
		"images/interface/detail salle/boutons hover.png",
		"images/interface/detail salle/boutons onclick.png",

		"images/interface/fusion salles/fond.png",
		"images/interface/fusion salles/bouton.png",
		"images/interface/fusion salles/bouton hover.png",
		"images/interface/fusion salles/bouton onclick.png",

		"images/interface/detail machine/fond.png",
		"images/interface/detail machine/progression.png",


		"images/salle des commandes/salle.png",
		"images/salle des commandes/connexion.png",

		"images/couloir I/horizontal/salle.png",
		"images/couloir I/horizontal/connexions.png",
		"images/couloir I/vertical/salle.png",
		"images/couloir I/vertical/connexions.png",

		"images/couloir T/haut/salle.png",
		"images/couloir T/haut/connexions.png",
		"images/couloir T/bas/salle.png",
		"images/couloir T/bas/connexions.png",
		"images/couloir T/droit/salle.png",
		"images/couloir T/droit/connexions.png",
		"images/couloir T/gauche/salle.png",
		"images/couloir T/gauche/connexions.png",

		"images/couloir L/haut/salle.png",
		"images/couloir L/haut/connexions.png",
		"images/couloir L/bas/salle.png",
		"images/couloir L/bas/connexions.png",
		"images/couloir L/droit/salle.png",
		"images/couloir L/droit/connexions.png",
		"images/couloir L/gauche/salle.png",
		"images/couloir L/gauche/connexions.png",
		
		"images/couloir X/salle.png",
		"images/couloir X/connexions.png",

		"images/salle de batteries/verticale/salle.png",
		"images/salle de batteries/verticale/connexions.png",
		"images/salle de batteries/horizontale/salle.png",
		"images/salle de batteries/horizontale/connexions.png",

		"images/centrale electrique/horizontale/salle.png",
		"images/centrale electrique/horizontale/connexions.png",
		"images/centrale electrique/verticale/salle.png",
		"images/centrale electrique/verticale/connexions.png",

		"images/panneau solaire/vertical/salle.png",
		"images/panneau solaire/vertical/connexions.png",
		"images/panneau solaire/horizontal/salle.png",
		"images/panneau solaire/horizontal/connexions.png",

		"images/pile radio isotopique/haut.png",
		"images/pile radio isotopique/bas.png",
		"images/pile radio isotopique/droite.png",
		"images/pile radio isotopique/gauche.png",

		"images/reserve eau/verticale/salle.png",
		"images/reserve eau/verticale/connexions.png",
		"images/reserve eau/verticale/reservoirs.png",
		"images/reserve eau/horizontale/salle.png",
		"images/reserve eau/horizontale/connexions.png",
		"images/reserve eau/horizontale/reservoirs.png",

		"images/salle de refroidissement/verticale/salle.png",
		"images/salle de refroidissement/verticale/connexions.png",
		"images/salle de refroidissement/verticale/terminal radiateur.png",
		"images/salle de refroidissement/horizontale/salle.png",
		"images/salle de refroidissement/horizontale/connexions.png",
		"images/salle de refroidissement/horizontale/terminal radiateur.png",

		"images/panneau radiateur/vertical/salle.png",
		"images/panneau radiateur/vertical/connexions.png",
		"images/panneau radiateur/horizontal/salle.png",
		"images/panneau radiateur/horizontal/connexions.png",


		"images/personnages/normal.png",
		"images/personnages/masque normal.png"
	};

	for (int i(0); i < nombreImages; i++)
	{
		m_textures[cheminsImages[i]].loadFromFile(cheminsImages[i]);
	}

	// Chargement des shaders

	int const nombreShaders(2);

	std::string cheminsShaders[nombreShaders] = {
		"shaders/mainSalles.frag",
		"shaders/masqueRessources.frag"
	};

	for (int i(0); i < nombreShaders; i++)
	{
		m_shaders[cheminsShaders[i]].loadFromFile(cheminsShaders[i], sf::Shader::Fragment);
	}

	// Chargement de la font

	m_font.loadFromFile("fonts/abel.ttf");
}


sf::Texture* Loader::obtenirTexture(std::string const& chemin)
{
	return &m_textures[chemin];
}


sf::Shader* Loader::obtenirShader(std::string const& chemin)
{
	return &m_shaders[chemin];
}


sf::Font* Loader::getFont()
{
	return &m_font;
}
