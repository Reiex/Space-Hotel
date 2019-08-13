#include "Entite.h"


Entite::Entite(): 
	m_x(0),
	m_y(0),
	m_w(1),
	m_h(1)
{
}


Entite::Entite(int x, int y, int w, int h):
	m_x(x),
	m_y(y)
{
    if(w > 0)
    {
        m_w = w;
    }
    else
    {
        m_w = 1;
    }

    if(h > 0)
    {
        m_h = h;
    }
    else
    {
        m_h = 1;
    }
}


Entite::Entite(sf::View const& view)
{
	m_x = view.getCenter().x - view.getSize().x / 2;
	m_y = view.getCenter().y - view.getSize().y / 2;
	m_w = view.getSize().x;
	m_h = view.getSize().y;
}


void Entite::setCoord(int x, int y, int w, int h)
{
    m_x = x;
    m_y = y;

    if(w > 0)
    {
        m_w = w;
    }
    if(h > 0)
    {
        m_h = h;
    }
}


bool Entite::collision(Entite const& entite) const
{
	return !((m_x >= entite.m_x + entite.m_w) || (entite.m_x >= m_x + m_w) || (m_y >= entite.m_y + entite.m_h) || (entite.m_y >= m_y + m_h));
}


bool Entite::collisionSouris(sf::RenderWindow const& window) const
{
    sf::Vector2<int> position_souris(sf::Mouse::getPosition(window));
    return m_x <= position_souris.x && position_souris.x < m_x + m_w && m_y <= position_souris.y && position_souris.y < m_y + m_h;
}


int Entite::getX() const
{
	return m_x;
}


int Entite::getY() const
{
	return m_y;
}


int Entite::getW() const
{
	return m_w;
}


int Entite::getH() const
{
	return m_h;
}
