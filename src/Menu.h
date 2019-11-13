#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include "Connexion.h"
#include <math.h>


// FENETRES


class FenetreDescription : public Entite
{
	public:

		FenetreDescription(Loader& loader, sf::VideoMode const& resolution, std::string const& chaine);

		void afficher(sf::RenderWindow& window) const;
		void setPosition(int x, int y);

		void setDeclencheur(Entite* entite);
		bool estActive(sf::RenderWindow const& window) const;

	private:

		sf::Texture m_texture;
		int m_xSprite;
		int m_ySprite;

		Entite* m_declencheur;
};



class FenetreInformation : public Entite
{
	public:

		FenetreInformation(Loader& loader, sf::VideoMode const& resolution, std::string const& chaine);

		void afficher(sf::RenderWindow& window);

		void ajusterDecalage();
		void ajusterDecalage(FenetreInformation& fenetre);

		bool estActive(sf::RenderWindow const& window);

	private:

		sf::Texture m_texture;
		int m_xSprite;
		int m_ySprite;

		sf::Clock m_tempsAffiche;
		float m_tempsAffichage;
};


// BOUTONS


class Bouton : public Entite
{
	public:

		virtual void afficher(sf::RenderWindow& window) const;
		void afficherDescription(sf::RenderWindow& window) const;

		void setDescription(FenetreDescription* description);
		void setSprite(sf::Sprite& sprite, sf::Sprite& spriteHover, sf::Sprite& spriteOnclick);

	protected:

		sf::Sprite m_sprite;
		sf::Sprite m_spriteHover;
		sf::Sprite m_spriteOnclick;

		FenetreDescription* m_description;
};


// MENUS


class Menu : public Entite
{
	public:

		void afficher(sf::RenderWindow& window) const;
		void afficherDescriptions(sf::RenderWindow& window) const;
		
		int getClicBouton(sf::RenderWindow const& window) const;

		void setAfficher(bool afficher);
		bool estAffiche() const;

	protected:

		std::vector<Bouton> m_boutons;
		std::vector<FenetreDescription> m_descriptions;
		sf::Sprite m_spriteFond;
		bool m_afficher;
};


class HudGauche : public Menu
{
	public:

		HudGauche(Loader& loader, sf::VideoMode const& resolution);
};


class HudDroit : public Menu
{
	public:

		HudDroit(Loader& loader, sf::VideoMode const& resolution);
};


class MenuCreationSalle : public Menu
{
	public:

		MenuCreationSalle(Loader& loader, sf::VideoMode const& resolution);
};


class MenuDetailSalle : public Menu
{
	public:

		MenuDetailSalle(Loader& loader, sf::VideoMode const& resolution);
};


class MenuFusionSalles : public Menu
{
	public:

		MenuFusionSalles(Loader& loader, sf::VideoMode const& resolution);
};


class MenuDetailMachine : public Menu
{
	public:

		MenuDetailMachine(Loader& loader, sf::VideoMode const& resolution);
};


class MenuCreationStation : public Menu
{
	public:

		MenuCreationStation(Loader& loader, sf::VideoMode const& resolution);
};


#endif // INTERFACE_H_INCLUDED