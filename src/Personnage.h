#ifndef PERSONNAGE_H_INCLUDED
#define PERSONNAGE_H_INCLUDED

#include "Ressources.h"
#include <queue>


class Noeud : public Entite
{
	public:

		enum Type {Normal, Connexion, Machine};

		Noeud();

		void connecter(Noeud* noeud);
		void trouverTrajectoire(Noeud* noeud, std::queue<Noeud*>& trajectoire);

		void setOccupe(bool occupe);
		bool estLibre() const;

		int getType() const;
		void setType(int type);

		~Noeud();

	private:

		int m_type;
		bool m_occupe;
		std::vector<Noeud*> m_voisins;
};


class Personnage : public Entite, public Conteneur
{
	public:

		enum Direction {Droite, Gauche};

		Personnage(Noeud* noeud);

		Noeud* getDepart();
		Noeud* getArrivee();

		void afficher(sf::RenderWindow& window, Loader& loader) const;
		void setDirection(int direction);

		void avancer(float dt, float distance);
		void ajusterTrajectoire(Noeud* noeud);
		void annulerTrajectoire();
		bool traverseNoeud(Noeud* noeud);

		bool estOccupe();
		float getAvancement() const;

	private:

		float m_avancement;
		std::queue<Noeud*> m_trajectoire;
		Noeud* m_arrete[2];

		int m_direction;
};


void interconnecterNoeuds(std::vector<Noeud*>* listeNoeuds);

#endif
