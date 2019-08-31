#ifndef SALLE_H_INCLUDED
#define SALLE_H_INCLUDED


#include "Menu.h"
#include <math.h>


// CLASSE COMMUNE A TOUTES LES SALLES


class Salle: public Entite
{
    public:

		static int const NombreDeFlags;
		enum FlagsAffichage {Creation, Selection, MultiSelection, Panne};

		Salle();

		void setEnergieConso(float energie);
		void setEnergieProduite(float energie);
		void setEnergieSolaireProduite(float energie);
		void setCapaciteAccu(float capacite);
		void setEnergieAccu(float energie);
		void setChaleurDissipee(float chaleur);
		void setPanneElectrique(bool panne);

		void faireFonctionner(float dt);

		bool appartenanceNoeud(Noeud* noeud) const;
		void deplacerPersonnage(Personnage* personnage, float dt) const;
		bool estTraversee(Personnage* personnage) const;
		bool contientPersonnage(Personnage* personnage) const;
		
		bool essayerConnecterSalle(Salle& nouvelleSalle) const;
		void connecterSalle(Salle& nouvelleSalle);
		void deconnecterSalle(Salle& salleConnectee);
		bool raccorderSalle(Salle& salleConnectee);

		virtual void effectuerRotation(Loader& loader);

		void updateTexture(Machine* machinePointee);
		void afficher(Loader& loader, sf::RenderWindow& window, std::vector<int>& flags, float temperature, Machine* machinePointee);
		void afficherNoeuds(sf::RenderWindow& window) const;

		std::vector<Connexion>* getConnexions();
		std::vector<Salle*>* getVoisines();
		std::vector<Machine*>* getMachines();

		Noeud* getNoeudNormal();

		float getEnergieConso() const;
		float getEnergieConsoMachine() const;
		float getEnergieProduite() const;
		float getEnergieSolaireProduite() const;
		float getCapaciteAccu() const;
		float getEnergieAccu() const;
		float getChaleurDissipeeMachines() const;
		float getChaleurDissipee() const;
		bool getPanneElectrique() const;

        ~Salle();

    protected:

		enum Orientation {Haut, Bas, Droite, Gauche, Verticale, Horizontale};

		static float const DistanceFavorableConnexion;

		bool m_panneElectrique;
        float m_energieConso;
		float m_energieProduite;
		float m_energieSolaireProduite;
		float m_capaciteAccu;
		float m_energieAccu;
		float m_chaleurDissipee;

		int m_positionRotation;
		bool m_impasse;
        std::vector<Salle*> m_voisines;
        std::vector<Connexion> m_connexions;
		bool m_root;
		
		sf::Texture* m_textureSalle;
		sf::Texture* m_textureConnexions;
		sf::RenderTexture m_texture;

		std::vector<Noeud*> m_noeuds;

		std::vector<Machine*> m_machines;
};


// CLASSES POUR CHACUNE DES SALLES


class SalleCommandes: public Salle
{
    public:

		SalleCommandes(Loader& loader);
};


class CouloirI: public Salle
{
	public:

		CouloirI(Loader& loader);

		void effectuerRotation(Loader& loader);
};


class CouloirT : public Salle
{
	public:

		CouloirT(Loader& loader);

		void effectuerRotation(Loader& loader);
};


class CouloirL : public Salle
{
	public:

		CouloirL(Loader& loader);

		void effectuerRotation(Loader& loader);
};


class CouloirX : public Salle
{
	public:

		CouloirX(Loader& loader);
};


class SalleDeBatteries : public Salle
{
public:

	SalleDeBatteries(Loader& loader);

	void effectuerRotation(Loader& loader);
};


class CentraleElectrique : public Salle
{
	public:

		CentraleElectrique(Loader& loader);

		void effectuerRotation(Loader& loader);
};


class PanneauSolaire : public Salle
{
	public:

		PanneauSolaire(Loader& loader);

		void effectuerRotation(Loader& loader);
};


class PileRadioIsotopique : public Salle
{
	public:

		PileRadioIsotopique(Loader& loader);

		void effectuerRotation(Loader& loader);
};


class ReserveEau : public Salle
{
	public:

		ReserveEau(Loader& loader);

		void effectuerRotation(Loader& loader);
};


class SalleTraitementEau : public Salle
{
	public:

		SalleTraitementEau(Loader& loader);

		void effectuerRotation(Loader& loader);
};


class SalleDeRefroidissement : public Salle
{
	public:

		SalleDeRefroidissement(Loader& loader);

		void effectuerRotation(Loader& loader);
};


class PanneauRadiateur : public Salle
{
	public:

		PanneauRadiateur(Loader& loader);

		void effectuerRotation(Loader& loader);
};


#endif // SALLE_H_INCLUDED
