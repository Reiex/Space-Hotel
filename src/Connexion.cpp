#include "Connexion.h"


Connexion::Connexion()
{
	m_direction = Connexion::Direction::Bas;

	m_connectee = false;
	m_salleConnectee = 0;
	m_type = Connexion::Type::Standard;
}


void Connexion::setType(int type)
{
    m_type = type;
}


void Connexion::setDirection(int direction)
{
	m_direction = direction;
}


void Connexion::setNoeud(Noeud* noeud)
{
	m_noeud = noeud;
}


bool Connexion::connexionCompatible(Connexion const& connexion) const
{
	return (m_direction + connexion.m_direction == 0 && connexion.m_type == m_type);
}


void Connexion::connecter()
{
	m_connectee = true;
}


void Connexion::connecter(Connexion& connexion)
{
	// La connexion se fait des deux côtés

	deconnecter();
	connexion.deconnecter();

	m_connectee = true;
	m_salleConnectee = &connexion;
	connexion.m_connectee = true;
	connexion.m_salleConnectee = this;

	if (m_noeud != 0 && connexion.m_noeud != 0)
	{
		m_noeud->connecter(connexion.m_noeud);
	}
}


void Connexion::deconnecter()
{
	m_connectee = false;

	if (m_salleConnectee != 0)
	{
		m_salleConnectee->m_connectee = false;
		m_salleConnectee->m_salleConnectee = 0;
		m_salleConnectee = 0;
	}
}


int Connexion::getDirection() const
{
	return m_direction;
}


bool Connexion::estConnectee() const
{
	return m_connectee;
}


Connexion* Connexion::getSalleConnectee() const
{
	return m_salleConnectee;
}