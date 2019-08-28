#ifndef RESSOURCES_H_INCLUDED
#define RESSOURCES_H_INCLUDED

#include "Entite.h"
#include "Loader.h"
#include "fonctions.h"
#include <vector>


class Noeud;
class Ressource;
class EmplacementRessource;


class Conteneur
{
	public:
		
		Conteneur(int nbEmplacement);

		bool ressourceNecessaire(Ressource* ressource) const;
		bool ressourceDeposable(Ressource* ressource) const;
		void deposerRessource(Ressource* ressource);

		bool ressourcePresente(int i) const;
		int getTypeRessource(int i) const;

		Ressource* getRessource(int i);
		void retirerRessource(Ressource* ressource);

		~Conteneur();

	protected:

		std::vector<EmplacementRessource> m_emplacements;
};


class EmplacementRessource
{
	public:

		EmplacementRessource(bool entree);

		void interdireRessource(int type);
		void autoriserRessource(int type);
		bool estAutorisee(Ressource* ressource) const;

		void setEntree(bool entree);
		void setRessourceNecessaire(bool necessaire);
		bool estOccupe() const;
		bool ressourceNecessaire() const;

		void setRessource(Ressource* ressource);
		Ressource* getRessource() const;

	private:

		Ressource* m_ressource;
		bool m_entree;
		bool m_ressourceNecessaire;
		std::vector<int> m_autorisees;
};


class Ressource : public Entite
{
	public:

		enum Type {Eau};
		static int const nbTypes;
		static std::string const cheminsTypes[];

		Ressource(int type);

		int getType() const;

		Conteneur* getConteneur() const;
		void setConteneur(Conteneur* conteneur);
		Noeud* getNoeudProche() const;
		void setNoeudProche(Noeud* noeud);

		float getEtat() const;
		void setEtat(float etat);

		~Ressource();

	private:

		Conteneur* m_conteneur;
		Noeud* m_noeudProche;
		int m_type;
		float m_etat;
};


class Machine : public Conteneur, public Entite
{
	public:

		static Entite const SurfaceDessinDetails;

		Machine(int nbEntrees, int nbSorties);
		virtual void faireFonctionner(float dt, bool panneElectrique) = 0;

		float getEnergieConso() const;
		float getChaleurDissipee() const;

		virtual void effectuerRotation(Loader& loader);

		virtual sf::Texture* getTexture();
		virtual void afficherDetails(sf::RenderWindow& window, Loader& loader, bool panneElectrique) const = 0;

		void setNoeudProche(Noeud* noeud);
		Noeud* getNoeudProche() const;

		~Machine();

	protected:

		Noeud* m_noeudProche;

		float m_energieConso;
		float m_chaleurDissipee;

		sf::Texture* m_texture;
};


// CLASSES POUR CHACUNE DES MACHINES


class TerminalRadiateur : public Machine
{
	public:

		TerminalRadiateur(Loader& loader);
		void faireFonctionner(float dt, bool panneElectrique);
		void effectuerRotation(Loader& loader);
		void afficherDetails(sf::RenderWindow& window, Loader& loader, bool panneElectrique) const;
};


class ReservoirEau : public Machine
{
	public:

		ReservoirEau(Loader& loader);
		void faireFonctionner(float dt, bool panneElectrique);
		void effectuerRotation(Loader& loader);
		sf::Texture* getTexture();
		void afficherDetails(sf::RenderWindow& window, Loader& loader, bool panneElectrique) const;

	private:

		sf::RenderTexture m_renderTexture;
		sf::Texture m_textureFinale;
		int m_remplissage[2];
		bool m_vertical;

};


#endif
