#include "fonctions.h"


void afficherFps(sf::RenderWindow& window, Loader* loader, float dt)
{
    int fps(1/dt);
    std::ostringstream flux_conversion;;

    flux_conversion << fps;
    sf::String s_fps(flux_conversion.str());

    sf::Text texte_fps(s_fps, *(loader->getFont()));
    texte_fps.setFillColor(sf::Color(0, 255, 0));
    texte_fps.setPosition(window.getView().getSize().x - 70, 0);

    window.draw(texte_fps);
}


sf::Texture creerTextureTexte(Loader& loader, int taille, std::string const& chaine)
{
	// Calculer la taille de la zone de texte

	int nbLignes(0);
	std::string strLigne("");
	int tailleLigne(0), tailleMax(0);
	sf::Text ligne;

	ligne.setFont(*(loader.getFont()));
	ligne.setCharacterSize(taille);

	for (int i(0); i < chaine.size(); i++)
	{
		if (chaine[i] == '\n')
		{
			ligne.setString(std::string(strLigne));
			tailleLigne += ligne.getGlobalBounds().width;
			if (tailleLigne > tailleMax)
			{
				tailleMax = tailleLigne;
			}
			nbLignes++;

			strLigne = "";
			tailleLigne = 0;
		}
		else if (chaine[i] == '{')
		{
			while (chaine[i] != '}')
			{
				i++;
			}
			ligne.setString(std::string(strLigne));
			tailleLigne += ligne.getGlobalBounds().width;
			tailleLigne += taille;

			strLigne = "";
		}
		else
		{
			strLigne += chaine[i];
		}
	}

	ligne.setString(std::string(strLigne));
	tailleLigne += ligne.getGlobalBounds().width;
	if (tailleLigne > tailleMax)
	{
		tailleMax = tailleLigne;
	}
	nbLignes++;
	strLigne = "";

	// Créer la texture vide

	sf::RenderTexture texture;
	texture.create(tailleMax + taille / 2, taille * (nbLignes * 1.275));
	texture.clear(sf::Color(0, 0, 0, 0));

	// Remplir la texture

	int nLigne(0);
	int positionCurseur(0);

	ligne.setFont(*(loader.getFont()));
	ligne.setCharacterSize(taille);
	ligne.setFillColor(sf::Color(28, 163, 39));

	for (int i(0); i < chaine.size(); i++)
	{
		if (chaine[i] == '\n')
		{
			ligne.setString(std::string(strLigne));
			ligne.setPosition(positionCurseur, int(nLigne * taille * 1.275));
			texture.draw(ligne);

			nLigne++;
			strLigne = "";
			positionCurseur = 0;
		}
		else if (chaine[i] == '{')
		{
			ligne.setString(std::string(strLigne));
			ligne.setPosition(positionCurseur, int(nLigne * taille * 1.275));
			texture.draw(ligne);

			positionCurseur += ligne.getGlobalBounds().width;

			i++;
			strLigne = "";
			while (chaine[i] != '}')
			{
				strLigne += chaine[i];
				i++;
			}
			
			sf::Sprite sprite(*(loader.obtenirTexture(strLigne)));
			sprite.setScale(float(taille) / loader.obtenirTexture(strLigne)->getSize().x, float(taille) / loader.obtenirTexture(strLigne)->getSize().y);
			sprite.setPosition(positionCurseur, int(nLigne * taille * 1.275) + 2);
			texture.draw(sprite);

			positionCurseur += taille;

			strLigne = "";
		}
		else
		{
			strLigne += chaine[i];
		}
	}

	ligne.setString(std::string(strLigne));
	ligne.setPosition(positionCurseur, int(nLigne * taille * 1.275));
	texture.draw(ligne);

	texture.display();

	return texture.getTexture();
}


sf::Texture creerTextureFenetre(Loader& loader, int taille, std::string const& chaine)
{
	int tailleBordure(5);

	// Créer le texte

	sf::Texture textureTexte(creerTextureTexte(loader, taille, chaine));

	// Obtenir la taille de la fenêtre

	int w(textureTexte.getSize().x + 2*tailleBordure), h(textureTexte.getSize().y + 2*tailleBordure);

	// Créer la texture

	sf::RenderTexture texture;
	texture.create(w, h);

	// Dessiner le cadre et le fond

	sf::RectangleShape fond(sf::Vector2f(w - 4, h - 4));
	fond.setFillColor(sf::Color(12, 51, 16));
	fond.setPosition(2, 2);
	texture.draw(fond);

	sf::Sprite sprite(*(loader.obtenirTexture("images/interface/fenetre.png")));

	sprite.setTextureRect(sf::IntRect(0, 0, 5, 5));
	sprite.setPosition(0, 0);
	texture.draw(sprite);

	sprite.setTextureRect(sf::IntRect(95, 0, 5, 5));
	sprite.setPosition(w - 5, 0);
	texture.draw(sprite);

	sprite.setTextureRect(sf::IntRect(0, 95, 5, 5));
	sprite.setPosition(0, h - 5);
	texture.draw(sprite);

	sprite.setTextureRect(sf::IntRect(95, 95, 5, 5));
	sprite.setPosition(w - 5, h - 5);
	texture.draw(sprite);

	sprite.setTextureRect(sf::IntRect(5, 0, 1, 5));
	for (int i(5); i < w - 5; i++)
	{
		sprite.setPosition(i, 0);
		texture.draw(sprite);
	}

	sprite.setTextureRect(sf::IntRect(0, 5, 5, 1));
	for (int i(5); i < h - 5; i++)
	{
		sprite.setPosition(0, i);
		texture.draw(sprite);
	}

	sprite.setTextureRect(sf::IntRect(5, 95, 1, 5));
	for (int i(5); i < w - 5; i++)
	{
		sprite.setPosition(i, h - 5);
		texture.draw(sprite);
	}

	sprite.setTextureRect(sf::IntRect(95, 5, 5, 1));
	for (int i(5); i < h - 5; i++)
	{
		sprite.setPosition(w - 5, i);
		texture.draw(sprite);
	}
	
	// Dessiner le texte et mettre à jour la texture

	sf::Sprite spriteTexte(textureTexte);
	spriteTexte.setPosition(tailleBordure + taille / 4, tailleBordure);

	texture.draw(spriteTexte);
	texture.display();

	return texture.getTexture();
}


std::string float_to_string(float a, int chiffresApresVirgule)
{
	std::string virgule(","), chaine;

	int facteur(1), puissance(0);
	while (int(a * facteur) != a * facteur && puissance <= 10)
	{
		facteur *= 10;
		puissance += 1;
	}

	if (facteur == 1)
	{
		chaine = std::to_string(int(a));
	}
	else
	{
		std::string chaineEntiere, chaineDecimales;
		int nbChiffres(std::to_string(int(a * facteur)).size());

		chaineEntiere = std::to_string(int(a)) + virgule;
		chaineDecimales = std::to_string(int(a * facteur) % facteur);

		chaine = chaineEntiere;
		for (int i(0); i < nbChiffres - int(chaineEntiere.size()) + 1 - int(chaineDecimales.size()); i++)
		{
			chaine += "0";
		}
		chaine += chaineDecimales;
	}

	int pos(-1);
	for (int i(0); i < chaine.size(); i++)
	{
		if (chaine[i] == ',')
		{
			pos = i;
			break;
		}
	}

	if (pos != -1 && chiffresApresVirgule != 0)
	{
		chaine = chaine.substr(0, pos + chiffresApresVirgule + 1);
	}

	return chaine;
}
