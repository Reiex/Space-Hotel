#ifndef LOADER_H_INCLUDED
#define LOADER_H_INCLUDED


#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <iostream>


class Loader
{
	public:

		Loader();
		sf::Texture* obtenirTexture(std::string const& chemin);
		sf::Shader* obtenirShader(std::string const& chemin);
		sf::Font* getFont();

	private:

		std::unordered_map<std::string, sf::Texture> m_textures;
		std::unordered_map<std::string, sf::Shader> m_shaders;
		sf::Font m_font;
};

#endif // LOADER_H_INCLUDED