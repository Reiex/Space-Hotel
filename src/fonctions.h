#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

#include <sstream>
#include "Loader.h"


void afficherFps(sf::RenderWindow& window, Loader* loader, float dt);


sf::Texture creerTextureTexte(Loader& loader, int taille, std::string const& chaine);


sf::Texture creerTextureFenetre(Loader& loader, int taille, std::string const& chaine);


std::string float_to_string(float a, int chiffresApresVirgule = 0);


#endif // FONCTIONS_H_INCLUDED
