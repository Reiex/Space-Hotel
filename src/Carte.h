#ifndef CARTE_H_INCLUDED
#define CARTE_H_INCLUDED


#include "Salle.h"
#include <cstdlib>
#include <ctime>


int emplacementSelection(std::vector<Salle*> const& sallesPointees, Salle* salle);


class Carte
{
	public:

		static int const DureeJour;

		Carte(sf::VideoMode const& resolution);

		void avancerTemps(float dt);
		float heureJournee() const;

		void afficher(sf::RenderWindow& window, std::vector<Salle*>& sallesPointees, Machine* machinePointee);
		void afficherRessources(sf::RenderWindow& window, Salle* sallePointee);
		void deplacerView(sf::RenderWindow const& window, float dt);
		void resetView();
		void gererZoom(float delta);

		void gererPersonnages(float dt);
		void gererIAPersonnages();
		void deplacerPersonnages(float dt);

		float energieProduite(float energie, float energieSolaire) const;
		float energieProduite(Salle* salle) const;
		float energieConsoMachines() const;
		float energieConsommee(float energie, float energieMachine) const;
		float energieConsommee(Salle* salle) const;
		void gererEnergie(float dt);

		float chaleurDissipeeMachines() const;
		void gererTemperature(float dt);

		void gererMachinesRessources(float dt);

		void gererClicMenu(sf::RenderWindow const& window, std::vector<Salle*>& sallesPointees, Salle*& nouvelleSalle);
		void ajouterInformation(std::string chaine);
		void gestionInformation(sf::RenderWindow const& window);

		bool collisionSourisMenu(sf::RenderWindow const& window);
		bool collisionSourisMachine(sf::RenderWindow const& window);

		Salle* preparerCreationSalle(sf::RenderWindow& window, Salle* nouvelleSalle);
		void ajouterSalle(Salle* nouvelleSalle, Salle* salleConnectee);
		bool enleverSalles(std::vector<Salle*>& sallesPointees);

		void selectionSalle(sf::RenderWindow const& window, std::vector<Salle*>& sallesPointees) const;
		void selectionMachine(sf::RenderWindow const& window, Machine*& machinePointee) const;

		Loader* getLoader();
		sf::View* getView();
		std::vector<Menu*>* getMenus();

		~Carte();

    private:
		
		float m_temps;
		float m_temperature;

		float m_energieConso;
		float m_energieProduite;
		float m_energieSolaireProduite;
		float m_energieAccu;
		float m_capaciteAccu;
		float m_chaleurDissipee;

		std::vector<Personnage*> m_personnages;
		std::vector<Ressource*> m_ressources;

        std::vector<Salle*> m_salles;
		std::vector<Menu*> m_menus;
		std::vector<FenetreInformation> m_fenetresInformation;
		Loader m_loader;

		sf::View m_viewJeu;
		sf::View m_viewInterface;
};


#endif // CARTE_H_INCLUDED
