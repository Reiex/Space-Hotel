#ifndef ASTEROIDE_H_INCLUDED
#define ASTEROIDE_H_INCLUDED

#include "Salle.h"


class Station;


class Asteroide : public Salle
{
	public:

		Asteroide(int rayon);
		void generer();

		bool collisionAsteroide(Asteroide* asteroide) const;
		void connecterStation(Station* nouvelleStation);
		void assurerConnectionSAS();
		bool estConnecteSalles() const;

		void afficher(Loader& loader, sf::RenderWindow& window, std::vector<int>& flags, float temperature, Machine* machinePointee);

		int getRayon() const;

	private:

		int m_rayon;
		bool m_connecteSalles;
};


class Station : public Machine
{
	public:

		Station(int nbEntrees, int nbSorties);

		void setAngle(float angle);
		float getAngle() const;
		void setDiametreAngulaire(float diametre);
		float getDiametreAngulaire() const;
		bool collisionAngulaire(Station* station);

	private:

		float m_angle;
		float m_diametreAngulaire;
};


class Foreuse : public Station
{
	public:

		Foreuse(Loader& loader);

		void faireFonctionner(float dt, bool panneElectrique);
		void afficherDetails(sf::RenderWindow& window, Loader& loader, bool panneElectrique) const;

	private:

		float m_avancement;
};


#endif // ASTEROIDE_H_INCLUDED