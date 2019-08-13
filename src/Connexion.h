#ifndef CONNEXION_H_INCLUDED
#define CONNEXION_H_INCLUDED


#include "Personnage.h"


class Connexion: public Entite
{
    public:
		
		enum Type {Standard, PanneauSolaire};
		enum Direction {Haut = 1, Bas = -Haut, Droite = 2, Gauche = -Droite};

		Connexion();

        void setType(int type);
		void setDirection(int direction);
		void setNoeud(Noeud* noeud);

		bool connexionCompatible(Connexion const& connexion) const;

		void connecter();
		void connecter(Connexion& connexion);
		void deconnecter();

		int getDirection() const;
		bool estConnectee() const;
		Connexion* getSalleConnectee() const;

    private:

        int m_direction;
		Noeud* m_noeud;

        bool m_connectee;
		Connexion* m_salleConnectee;

        int m_type;
};


#endif // CONNEXION_H_INCLUDED
