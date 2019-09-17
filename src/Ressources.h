#ifndef RESSOURCES_H_INCLUDED
#define RESSOURCES_H_INCLUDED

#include "Entite.h"
#include "fonctions.h"
#include <vector>


class Noeud;
class Ressource;
class EmplacementRessource;


class Conteneur
{
	public:
		
		Conteneur(int nbEmplacement);

		bool ressourceNecessaire(Ressource* ressource, int i) const;
		bool ressourceDeposable(Ressource* ressource, int i) const;
		void deposerRessource(Ressource* ressource, int i);

		int getTaille() const;

		bool ressourcePresente(int i) const;
		bool ressourceDisponnible(int i) const;
		bool ressourceEnTrop(int i) const;
		int getTypeRessource(int i) const;

		Ressource* getRessource(int i);
		void retirerRessource(Ressource* ressource);

		void reserverEmplacement(int i);
		void libererEmplacement(int i);
		bool emplacementLibre(int i) const;

		~Conteneur();

	protected:

		std::vector<EmplacementRessource> m_emplacements;
		std::vector<bool> m_masqueUtilisation;
};


class EmplacementRessource
{
	public:

		EmplacementRessource(bool entree);

		void interdireRessource(int type);
		void autoriserRessource(int type);
		bool estAutorisee(Ressource* ressource) const;

		void setEntree(bool entree);
		bool getEntree() const;
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

		enum Type {Eau, EauSale, Minerai, Metal};
		static int const nbTypes;
		static std::string const cheminsTypes[];

		Ressource(int type);

		void setType(int type);
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


class FiltreEau : public Machine
{
	public:

		FiltreEau(Loader& loader);
		void faireFonctionner(float dt, bool panneElectrique);
		void effectuerRotation(Loader& loader);
		void afficherDetails(sf::RenderWindow& window, Loader& loader, bool panneElectrique) const;

	private:

		float m_avancement;
};


class TerminalRadiateur : public Machine
{
	public:

		TerminalRadiateur(Loader& loader);
		void faireFonctionner(float dt, bool panneElectrique);
		void effectuerRotation(Loader& loader);
		void afficherDetails(sf::RenderWindow& window, Loader& loader, bool panneElectrique) const;
};


class Stock : public Machine
{
	public:

		Stock(Loader& loader);
		void faireFonctionner(float dt, bool panneElectrique);
		void effectuerRotation(Loader& loader);
		void afficherDetails(sf::RenderWindow& window, Loader& loader, bool panneElectrique) const;
};


class Four : public Machine
{
	public:

		Four(Loader& loader);
		void faireFonctionner(float dt, bool panneElectrique);
		void effectuerRotation(Loader& loader);
		void afficherDetails(sf::RenderWindow& window, Loader& loader, bool panneElectrique) const;

	private:

		float m_avancement;

};


#endif
