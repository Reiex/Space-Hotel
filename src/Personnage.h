#ifndef PERSONNAGE_H_INCLUDED
#define PERSONNAGE_H_INCLUDED

#include "Ressources.h"
#include <queue>


class Noeud : public Entite
{
	public:

		enum Type {Normal, Connexion, Machine, SAS};

		Noeud();

		int getDegre() const;
		void connecter(Noeud* noeud);
		void trouverTrajectoire(Noeud* noeud, std::queue<Noeud*>& trajectoire);

		void setOccupe(bool occupe);
		bool estLibre() const;

		int getType() const;
		void setType(int type);

		std::vector<Noeud*>* getVoisins();

		~Noeud();

	private:

		int m_type;
		bool m_occupe;
		int m_cible;
		std::vector<Noeud*> m_voisins;
};


class Personnage : public Entite, public Conteneur
{
	public:

		enum Direction {Droite, Gauche};
		enum Action{Ballade, DeposerRessource, PrendreRessource};

		Personnage(Noeud* noeud);

		Noeud* getDepart();
		Noeud* getArrivee();

		void afficher(sf::RenderWindow& window, Loader& loader) const;
		void setDirection(int direction);

		void avancer(float dt, float distance);
		void ajusterTrajectoire(Noeud* noeud);
		void annulerTrajectoire();
		bool traverseNoeud(Noeud* noeud);

		Machine* getMachineCible() const;
		void setMachineCible(Machine* machine);
		int getEmplacementCible() const;
		void setEmplacementCible(int emplacement);
		Action getAction() const;
		void setAction(Action action);

		bool estOccupe();
		float getAvancement() const;

	private:

		float m_avancement;
		std::queue<Noeud*> m_trajectoire;
		Noeud* m_arrete[2];

		int m_direction;

		Machine* m_machineCible;
		int m_emplacementCible;
		Action m_action;
};


void interconnecterNoeuds(std::vector<Noeud*>* listeNoeuds);

#endif
