#include "Asteroide.h"


// FONCTIONS MEMBRES DE LA CLASSE ASTEROIDE


Asteroide::Asteroide(int rayon)
{
	m_energieConso = 0;
	m_chaleurDissipee = 0;

	m_rayon = rayon;
	m_connecteSalles = false;
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
		contour.setPoint(i, sf::Vector2f((m_rayon + h) * cos(6.28 * i / nbPoints + phase), (m_rayon + h) * sin(6.28 * i / nbPoints + phase)));
		interieur.setPoint(i, sf::Vector2f((m_rayon + hInt) * cos(6.28 * i / nbPoints + phase), (m_rayon + hInt) * sin(6.28 * i / nbPoints + phase)));
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


void Asteroide::connecterStation(Station* nouvelleStation)
{
	m_machines.push_back(nouvelleStation);

	m_noeuds.push_back(new Noeud);
	m_noeuds[m_noeuds.size() - 1]->setType(Noeud::Machine);
	m_noeuds[m_noeuds.size() - 1]->setCoord(nouvelleStation->getX() + nouvelleStation->getW() / 2, nouvelleStation->getY() + nouvelleStation->getH() / 2);

	nouvelleStation->setNoeudProche(m_noeuds[m_noeuds.size() - 1]);

	m_connecteSalles = true;
}


void Asteroide::assurerConnectionSAS()
{
	for (int i(0); i < m_noeuds.size(); i++)
	{
		if (m_noeuds[i]->getDegre() != SAS::ListeSAS.size())
		{
			for (int j(0); j < SAS::ListeSAS.size(); j++)
			{
				m_noeuds[i]->connecter(SAS::ListeSAS[j]->getNoeudSortieSAS());
			}
		}
	}

	if (m_voisines.size() != SAS::ListeSAS.size())
	{
		m_voisines.clear();

		for (int i(0); i < SAS::ListeSAS.size(); i++)
		{
			m_voisines.push_back(SAS::ListeSAS[i]);

			std::vector<Salle*>* voisinesSAS(SAS::ListeSAS[i]->getVoisines());
			bool asteroidePlace(false);
			for (int j(0); j < voisinesSAS->size(); j++)
			{
				if ((*voisinesSAS)[j] == this)
				{
					asteroidePlace = true;
				}
			}

			if (!asteroidePlace)
			{
				voisinesSAS->push_back(this);
			}
		}
	}
}


bool Asteroide::estConnecteSalles() const
{
	return m_connecteSalles;
}


void Asteroide::afficher(Loader& loader, sf::RenderWindow& window, std::vector<int>& flags, float temperature, Machine* machinePointee)
{
	for (int i(0); i < m_machines.size(); i++)
	{
		Station* station((Station*) m_machines[i]);

		sf::Sprite sprite(*station->getTexture());
		if (station == machinePointee)
		{
			sprite.setColor(sf::Color(0, 255, 0));
		}
		sprite.setOrigin(station->getW()/2, station->getH()/2);
		sprite.setPosition(m_x + station->getX() + station->getW()/2, m_y + station->getY() + station->getH()/2);
		sprite.setRotation(-station->getAngle()*57.3);
		window.draw(sprite);
	}

	// Afficher l'asteroide lui-même

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


void Station::setAngle(float angle)
{
	m_angle = angle;
}


float Station::getAngle() const
{
	return m_angle;
}


void Station::setDiametreAngulaire(float diametre)
{
	m_diametreAngulaire = diametre;
}


float Station::getDiametreAngulaire() const
{
	return m_diametreAngulaire;
}


bool Station::collisionAngulaire(Station* station)
{
	float ecartAngulaire(m_angle - station->m_angle);
	while (ecartAngulaire < 0)
	{
		ecartAngulaire += 6.2831853;
	}
	while (ecartAngulaire >= 6.2831853)
	{
		ecartAngulaire -= 6.2831853;
	}

	if (6.2831853 - ecartAngulaire < ecartAngulaire)
	{
		ecartAngulaire = 6.2831853 - ecartAngulaire;
	}

	return ecartAngulaire < (m_diametreAngulaire / 2) + (station->m_diametreAngulaire / 2);
}


// FONCTIONS MEMBRES DE LA CLASSE FOREUSE


Foreuse::Foreuse(Loader& loader) : Station(0, 3)
{
	m_texture = loader.obtenirTexture("images/foreuse/foreuse.png");

	m_w = m_texture->getSize().x;
	m_h = m_texture->getSize().y;

	m_avancement = 0;
}


void Foreuse::faireFonctionner(float dt, bool panneElectrique)
{

	if (m_emplacements[0].getRessource() != 0 && m_emplacements[1].getRessource() != 0 && m_emplacements[2].getRessource() != 0)
	{
		m_avancement = 0;
	}
	else
	{
		m_avancement += dt / 60;
		if (m_avancement >= 1)
		{
			Ressource* ressource(new Ressource(Ressource::Minerai));
			if (m_emplacements[0].getRessource() == 0)
			{
				m_emplacements[0].setRessource(ressource);
			}
			else if (m_emplacements[1].getRessource() == 0)
			{
				m_emplacements[1].setRessource(ressource);
			}
			else
			{
				m_emplacements[2].setRessource(ressource);
			}
			m_avancement = 0;
		}
	}
}


void Foreuse::afficherDetails(sf::RenderWindow& window, Loader& loader, bool panneElectrique) const
{
	int x(SurfaceDessinDetails.getX()), y(SurfaceDessinDetails.getY()), w(SurfaceDessinDetails.getW()), h(SurfaceDessinDetails.getH());

	sf::Texture* texture;
	sf::Sprite sprite;
	sf::Text texte;
	texte.setFont(*(loader.getFont()));
	texte.setFillColor(sf::Color(5, 15, 6));
	texte.setCharacterSize(25);

	/*
		Avancement du minage:

				  ===>

				[] [] []

	*/

	texte.setString(sf::String("Avancement du minage:"));
	texte.setPosition(x, y + (h - 110) / 2);
	window.draw(texte);

	texture = loader.obtenirTexture("images/interface/detail machine/progression.png");
	sprite.setTexture(*texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 84, 30));
	sprite.setPosition(x + (w - 84) / 2, y + (h - 110) / 2 + 40);
	sprite.setScale(1, 1);
	window.draw(sprite);

	sprite.setTextureRect(sf::IntRect(0, 30, 75 * m_avancement, 22));
	sprite.setPosition(x + (w - 84) / 2 + 3, y + (h - 110) / 2 + 44);
	window.draw(sprite);

	for (int i(0); i < 3; i++)
	{
		if (m_emplacements[i].getRessource() != 0)
		{
			texture = loader.obtenirTexture(Ressource::cheminsTypes[m_emplacements[i].getRessource()->getType()]);
		}
		else
		{
			texture = loader.obtenirTexture("images/interface/logo ressource manquante.png");
		}
		sprite.setTexture(*texture);
		sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
		sprite.setPosition(x + (w - 110) / 2 + 40*i, y + (h - 110) / 2 + 80);
		sprite.setScale(0.3, 0.3);
		window.draw(sprite);
	}
}
