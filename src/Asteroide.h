#ifndef ASTEROIDE_H_INCLUDED
#define ASTEROIDE_H_INCLUDED

#include "Salle.h"


class Asteroide : public Salle
{
	public:

		Asteroide(int rayon);
		void generer();
		bool collisionAsteroide(Asteroide* asteroide) const;
		void afficher(Loader& loader, sf::RenderWindow& window, std::vector<int>& flags, float temperature, Machine* machinePointee);
		int getRayon() const;

	private:

		int m_rayon;
};


class Station : public Machine
{
	public:

		Station(int nbEntrees, int nbSorties);
};


class Foreuse : public Station
{
	public:

		Foreuse(Loader& loader);

		void faireFonctionner(float dt, bool panneElectrique);
		void afficherDetails(sf::RenderWindow& window, Loader& loader, bool panneElectrique) const;
};


#endif // ASTEROIDE_H_INCLUDED