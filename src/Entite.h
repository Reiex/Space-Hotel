#ifndef ENTITE_H_INCLUDED
#define ENTITE_H_INCLUDED

#include <SFML/Graphics.hpp>

class Entite
{
    public:

        Entite();
        Entite(int x, int y, int w = 0, int h = 0);
		Entite(sf::View const& view);

        void setCoord(int x, int y, int w = 0, int h = 0);

        bool collision(Entite const& entite) const;
        bool collisionSouris(sf::RenderWindow const& window) const;

		int getX() const;
		int getY() const;
		int getW() const;
		int getH() const;

    protected:

        int m_x;
        int m_y;
        int m_w;
        int m_h;
};

#endif // ENTITE_H_INCLUDED
