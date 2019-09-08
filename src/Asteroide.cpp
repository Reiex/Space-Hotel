#include "Asteroide.h"
#include <iostream>


// FONCTIONS MEMBRES DE LA CLASSE ASTEROIDE


Asteroide::Asteroide(int rayon)
{
	m_energieConso = 0;
	m_chaleurDissipee = 0;

	m_rayon = rayon;
}


void Asteroide::generer()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	m_texture.create(m_rayon * 2 + 100, m_rayon * 2 + 100, settings);
	m_texture.setSmooth(true);

	int h(35), hInt(25), nbPoints(100), phase(rand() % 10);
	sf::ConvexShape contour(nbPoints), interieur(nbPoints);
	for (float i(0); i < nbPoints; i++)
	{
		contour.setPoint(i, sf::Vector2f((m_rayon + h)*cos(6.28*i/nbPoints + phase), (m_rayon + h)*sin(6.28*i/nbPoints + phase)));
		interieur.setPoint(i, sf::Vector2f((m_rayon + hInt)*cos(6.28*i/nbPoints + phase), (m_rayon + hInt)*sin(6.28*i/nbPoints + phase)));
		h += rand() % 15 - 7;
		if (h > 50)
		{
			h = 48;
		}
		if (h < 20)
		{
			h = 22;
		}
		hInt = h - 10 - rand() % 10;
	}
	contour.setPosition(m_rayon + 50, m_rayon + 50);
	contour.setFillColor(sf::Color(127, 85, 61));
	interieur.setPosition(m_rayon + 50, m_rayon + 50);
	interieur.setFillColor(sf::Color(173, 116, 83));

	m_texture.draw(contour);
	m_texture.draw(interieur);
	m_texture.display();
}


bool Asteroide::collisionAsteroide(Asteroide* asteroide) const
{
	float distCarre(pow(m_x + m_rayon - asteroide->m_x - asteroide->m_rayon, 2) + pow(m_y + m_rayon - asteroide->m_y - asteroide->m_rayon, 2));
	return distCarre < pow(m_rayon + asteroide->m_rayon, 2);
}


void Asteroide::afficher(Loader& loader, sf::RenderWindow& window, std::vector<int>& flags, float temperature, Machine* machinePointee)
{
	sf::Sprite sprite(m_texture.getTexture());
	sprite.setPosition(m_x, m_y);

	window.draw(sprite);
}


int Asteroide::getRayon() const
{
	return m_rayon;
}


// FONCTIONS MEMBRES DE LA CLASSE STATION


Station::Station(int nbEntrees, int nbSorties) : Machine(nbEntrees, nbSorties)
{

}


// FONCTIONS MEMBRES DE LA CLASSE FOREUSE


Foreuse::Foreuse(Loader& loader) : Station(0, 0)
{
	m_texture = loader.obtenirTexture("images/foreuse/foreuse.png");

	m_w = 300;
	m_h = 100;
}


void Foreuse::faireFonctionner(float dt, bool panneElectrique)
{

}


void Foreuse::afficherDetails(sf::RenderWindow& window, Loader& loader, bool panneElectrique) const
{

}
