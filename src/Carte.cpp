#include "Carte.h"


int emplacementSelection(std::vector<Salle*> const& sallesPointees, Salle* salle)
{
	for (int i(0); i < sallesPointees.size(); i++)
	{
		if (sallesPointees[i] == salle)
		{
			return i;
		}
	}

	return -1;
}


int const Carte::DureeJour(60*5);


Carte::Carte(sf::VideoMode const& resolution)
{
	// Ajouter la salle de départ

	m_salles.push_back(new SalleCommandes(m_loader));

	// Ajouter les personnages de départ

	for (int i(0); i < 10; i++)
	{
		m_personnages.push_back(new Personnage(m_salles[0]->getNoeudNormal()));
		m_ressources.push_back(new Ressource(rand() % 2));
		m_personnages[i]->deposerRessource(m_ressources[m_ressources.size() - 1], 0);
	}

	// Initialiser les ressources

	m_energieConso = m_salles[0]->getEnergieConso();
	m_energieProduite = m_salles[0]->getEnergieProduite();
	m_energieSolaireProduite = m_salles[0]->getEnergieSolaireProduite();
	m_capaciteAccu = m_salles[0]->getCapaciteAccu();
	m_energieAccu = m_salles[0]->getEnergieAccu();
	m_chaleurDissipee = m_salles[0]->getChaleurDissipee();

	// Initialiser la température et le temps

	m_temps = 0;
	m_temperature = 15;

	// Initialiser les view

	m_viewJeu.reset(sf::FloatRect(0, 0, resolution.width, resolution.height));
	m_viewJeu.setCenter(0, 0);

	m_viewInterface.reset(sf::FloatRect(0, 0, resolution.width, resolution.height));

	// Initialiser les chunks d'asteroides decouverts

	m_chunksDecouverts.push_back(std::pair<int, int>(0, 0));

	// Ajouter les menus

	m_menus.push_back(new HudGauche(m_loader, resolution));
	m_menus.push_back(new HudDroit(m_loader, resolution));
	m_menus.push_back(new MenuCreationSalle(m_loader, resolution));
	m_menus.push_back(new MenuDetailSalle(m_loader, resolution));
	m_menus.push_back(new MenuFusionSalles(m_loader, resolution));
	m_menus.push_back(new MenuDetailMachine(m_loader, resolution));
	m_menus.push_back(new MenuCreationStation(m_loader, resolution));
}


void Carte::avancerTemps(float dt)
{
	m_temps += dt;
}


float Carte::heureJournee() const
{
	return int(m_temps) % (2 * Carte::DureeJour) + m_temps - int(m_temps);
}


void Carte::afficher(sf::RenderWindow& window, std::vector<Salle*>& sallesPointees, Machine* machinePointee)
{
	// Afficher le fond

	window.setView(sf::View(sf::FloatRect(0, 0, 1920, 1080)));

	sf::Texture* texture;
	sf::Sprite sprite;

	texture = m_loader.obtenirTexture("images/fond/fond.png");
	sprite.setTexture(*texture);
	sprite.setPosition(0, 0);

	window.draw(sprite);

	float x(m_viewJeu.getCenter().x), y(m_viewJeu.getCenter().y), w(m_viewJeu.getSize().x), h(m_viewJeu.getSize().y), wBis, hBis;

	texture = m_loader.obtenirTexture("images/fond/parallax0.png");
	sprite.setTexture(*texture);
	wBis = 1920*((w/1920)*0.01 + 1);
	hBis = 1080*((h/1080)*0.01 + 1);
	sprite.setTextureRect(sf::IntRect(int(x / 100) % 3840 - wBis/2, int(y / 100) % 2160 - hBis/2, wBis, hBis));
	sprite.setScale(1920.0 / wBis, 1080.0 / hBis);
	sprite.setPosition(0, 0);
	if (!texture->isRepeated() || !texture->isSmooth())
	{
		texture->setRepeated(true);
		texture->setSmooth(true);
	}
	window.draw(sprite);

	texture = m_loader.obtenirTexture("images/fond/parallax1.png");
	sprite.setTexture(*texture);
	wBis = 1920*((w/1920)*0.1 + 1);
	hBis = 1080*((h/1080)*0.1 + 1);
	sprite.setTextureRect(sf::IntRect(int(x / 10) % 3840 - wBis/2, int(y / 10) % 2160 - hBis/2, wBis, hBis));
	sprite.setScale(1920.0 / wBis, 1080.0 / hBis);
	sprite.setPosition(0, 0);
	if (!texture->isRepeated() || !texture->isSmooth())
	{
		texture->setRepeated(true);
		texture->setSmooth(true);
	}
	window.draw(sprite);

	// Afficher les salles

	window.setView(m_viewJeu);
	Entite fenetreCollision(m_viewJeu);

	for (int i(0); i < m_salles.size(); i++)
	{
		std::vector<int> flags;

		if (m_salles[i]->collision(fenetreCollision))
		{
			int const position(emplacementSelection(sallesPointees, m_salles[i]));

			if (position == -1)
			{
				if (m_salles[i]->getPanneElectrique())
				{
					flags.push_back(Salle::FlagsAffichage::Panne);
				}
			}
			else if (position == 0)
			{
				flags.push_back(Salle::FlagsAffichage::Selection);
			}
			else
			{
				flags.push_back(Salle::FlagsAffichage::MultiSelection);
			}

			m_salles[i]->afficher(m_loader, window, flags, m_temperature, machinePointee);
			// m_salles[i]->afficherNoeuds(window);
		}
	}

	// Afficher les asteroides

	for (int i(0); i < m_asteroides.size(); i++)
	{
		std::vector<int> flags;
		if (m_asteroides[i]->collision(fenetreCollision) && !m_asteroides[i]->estConnecteSalles())
		{
			m_asteroides[i]->afficher(m_loader, window, flags, m_temperature, machinePointee);
		}
	}

	// Afficher les ressources

	for (int i(0); i < m_ressources.size(); i++)
	{
		if (m_ressources[i]->getConteneur() == 0)
		{
			int type(m_ressources[i]->getType());
			sf::Texture* texture(m_loader.obtenirTexture(Ressource::cheminsTypes[type]));
			sf::Sprite sprite(*texture);
			sf::Vector2u taille(texture->getSize());
			sprite.setScale(23.0 / taille.x, 23.0 / taille.y);
			sprite.setPosition(m_ressources[i]->getX(), m_ressources[i]->getY());

			window.draw(sprite);
		}
	}

	// Afficher les personnages

	for (int i(0); i < m_personnages.size(); i++)
	{
		m_personnages[i]->afficher(window, m_loader);
	}

	// Afficher les menus

	window.setView(sf::View(sf::FloatRect(0, 0, 1920, 1080)));

	m_menus[4]->setAfficher(sallesPointees.size() == 2);

	for (int i(0); i < m_menus.size(); i++)
	{
		if (m_menus[i]->estAffiche())
		{
			m_menus[i]->afficher(window);
		}
	}
	
	if (sallesPointees.size() != 0)
	{
		afficherRessources(window, sallesPointees[0], machinePointee);
	}
	else
	{
		afficherRessources(window, 0, machinePointee);
	}

	// Afficher les descriptions

	for (int i(0); i < m_menus.size(); i++)
	{
		if (m_menus[i]->estAffiche())
		{
			m_menus[i]->afficherDescriptions(window);
		}
	}

	// Afficher les fenetres d'informations

	for (int i(0); i < m_fenetresInformation.size(); i++)
	{
		m_fenetresInformation[i].afficher(window);
	}

	window.setView(m_viewInterface);
}


void Carte::afficherRessources(sf::RenderWindow& window, Salle* sallePointee, Machine* machinePointee)
{
	// AFFICHER LES RESSOURCES GLOBALES DANS LE HUD

	if (m_menus[1]->estAffiche())
	{
		sf::Texture* texture;
		sf::Sprite sprite;
		std::string chaine;
		sf::Text texte;

		sprite.setScale(0.3, 0.3);

		texte.setCharacterSize(30);
		texte.setFillColor(sf::Color(5, 15, 6));
		texte.setFont(*(m_loader.getFont()));

		// Consommation electrique

		texture = m_loader.obtenirTexture("images/interface/logo electricite.png");
		sprite.setTexture(*texture);
		sprite.setPosition(1780, 887);
		window.draw(sprite);

		float energieConso(m_energieConso + energieConsoMachines());
		chaine = std::to_string(int(100 * energieConso / energieProduite(m_energieProduite, m_energieSolaireProduite)));
		chaine += "%";
		texte.setString(sf::String(chaine));
		texte.setPosition(1815, 885);
		window.draw(texte);

		// Capacite electrique

		texture = m_loader.obtenirTexture("images/interface/logo capacite.png");
		sprite.setTexture(*texture);
		sprite.setPosition(1780, 847);
		window.draw(sprite);

		if (m_capaciteAccu > 0)
		{
			chaine = std::to_string(int(100 * m_energieAccu / m_capaciteAccu));
		}
		else
		{
			chaine = std::to_string(0);
		}
		chaine += "%";
		texte.setString(sf::String(chaine));
		texte.setPosition(1815, 845);
		window.draw(texte);

		// Nombre d'habitants de la station

		texture = m_loader.obtenirTexture("images/interface/logo personnage.png");
		sprite.setTexture(*texture);
		sprite.setPosition(1780, 807);
		window.draw(sprite);

		chaine = std::to_string(m_personnages.size());
		texte.setString(sf::String(chaine));
		texte.setPosition(1815, 805);
		window.draw(texte);

		// Température

		texture = m_loader.obtenirTexture("images/interface/logo temperature.png");
		sprite.setTexture(*texture);
		sprite.setPosition(1780, 767);
		window.draw(sprite);

		chaine = float_to_string(m_temperature, 1);
		chaine += "°C";
		texte.setString(sf::String(chaine));
		texte.setPosition(1815, 765);
		window.draw(texte);

		// Temps

		float heure(heureJournee());
		if (heure < Carte::DureeJour)
		{
			texture = m_loader.obtenirTexture("images/interface/logo jour.png");
		}
		else
		{
			texture = m_loader.obtenirTexture("images/interface/logo nuit.png");
			heure -= Carte::DureeJour;
		}
		sprite.setTexture(*texture);
		sprite.setPosition(1780, 925);
		window.draw(sprite);

		texture = m_loader.obtenirTexture("images/interface/HUD droit/progression.png");
		sprite.setTexture(*texture);
		sprite.setTextureRect(sf::IntRect(0, 0, 95, 30));
		sprite.setPosition(1815, 925);
		sprite.setScale(1, 1);
		window.draw(sprite);
		
		sprite.setTextureRect(sf::IntRect(0, 30, 87 * heure / Carte::DureeJour, 22));
		sprite.setPosition(1819, 929);
		window.draw(sprite);
	}

	// AFFICHER LES RESSOURCES DE LA SALLE SELECTIONNEE

	if (m_menus[3]->estAffiche())
	{
		if (sallePointee == 0)
		{
			m_menus[3]->setAfficher(false);
			m_menus[0]->setAfficher(true);
			return;
		}

		sf::Texture* texture;
		sf::Sprite sprite;
		std::string chaine;
		sf::Text texte;

		sprite.setScale(0.3, 0.3);

		texte.setCharacterSize(30);
		texte.setFillColor(sf::Color(5, 15, 6));
		texte.setFont(*(m_loader.getFont()));

		// Production electrique

		texture = m_loader.obtenirTexture("images/interface/logo electricite.png");
		sprite.setTexture(*texture);
		sprite.setPosition(15, 127);
		window.draw(sprite);

		chaine = float_to_string(energieProduite(sallePointee), 1);
		texte.setString(sf::String(chaine));
		texte.setPosition(50, 125);
		window.draw(texte);

		// Consommation electrique

		texture = m_loader.obtenirTexture("images/interface/logo electricite.png");
		sprite.setTexture(*texture);
		sprite.setPosition(15, 167);
		window.draw(sprite);

		chaine = float_to_string(sallePointee->getEnergieConso());
		texte.setString(sf::String(chaine));
		texte.setPosition(50, 165);
		window.draw(texte);

		// Capacite electrique

		texture = m_loader.obtenirTexture("images/interface/logo capacite.png");
		sprite.setTexture(*texture);
		sprite.setPosition(15, 207);
		window.draw(sprite);

		chaine = float_to_string(sallePointee->getCapaciteAccu());
		texte.setString(sf::String(chaine));
		texte.setPosition(50, 205);
		window.draw(texte);

		// Energie accumulée

		texture = m_loader.obtenirTexture("images/interface/logo capacite.png");
		sprite.setTexture(*texture);
		sprite.setPosition(15, 247);
		window.draw(sprite);

		if (sallePointee->getCapaciteAccu() != 0)
		{
			chaine = std::to_string(int(100 * sallePointee->getEnergieAccu() / sallePointee->getCapaciteAccu()));
		}
		else
		{
			chaine = std::to_string(0);
		}
		chaine += "%";
		texte.setString(sf::String(chaine));
		texte.setPosition(50, 245);
		window.draw(texte);

		// Chaleur dissipée

		texture = m_loader.obtenirTexture("images/interface/logo temperature.png");
		sprite.setTexture(*texture);
		sprite.setPosition(15, 287);
		window.draw(sprite);

		if (sallePointee->getPanneElectrique())
		{
			chaine = "0 (" + float_to_string(sallePointee->getChaleurDissipee()) + ")";
		}
		else
		{
			chaine = float_to_string(sallePointee->getChaleurDissipee());
		}
		texte.setString(sf::String(chaine));
		texte.setPosition(50, 285);
		window.draw(texte);
	}

	// AFFICHER LES DETAILS DE LA MACHINE SELECTIONNEE

	if (m_menus[5]->estAffiche())
	{
		Salle* salleMachine(m_salles[0]);
		for (int i(0); i < m_salles.size(); i++)
		{
			std::vector<Machine*>* machines(m_salles[i]->getMachines());

			for (int j(0); j < machines->size(); j++)
			{
				if ((*machines)[j] == machinePointee)
				{
					salleMachine = m_salles[i];
					i = m_salles.size();
					break;
				}
			}
		}

		machinePointee->afficherDetails(window, m_loader, salleMachine->getPanneElectrique());
	}
}


void Carte::deplacerView(sf::RenderWindow const& window, float dt)
{
	float deplacementX(0), deplacementY(0);
	sf::Vector2i positionSouris(sf::Mouse::getPosition(window));
	sf::Vector2f resolution(m_viewInterface.getSize()), tailleJeu(m_viewJeu.getSize());
	float facteur(tailleJeu.x / resolution.x);

	if (positionSouris.x < 5)
	{
		deplacementX = positionSouris.x - 5;
	}
	else if(resolution.x - positionSouris.x < 5)
	{
		deplacementX = 5 - resolution.x + positionSouris.x;
	}

	if (positionSouris.y < 5)
	{
		deplacementY = positionSouris.y - 5;
	}
	else if (resolution.y - positionSouris.y < 5)
	{
		deplacementY = 5 - resolution.y + positionSouris.y;
	}

	m_viewJeu.move(300 * facteur * deplacementX*dt, 300 * facteur * deplacementY*dt);

	decouvrirAsteroides();
}


void Carte::resetView()
{
	m_viewJeu.setCenter(0, 0);
	m_viewJeu.setSize(m_viewInterface.getSize());
}


void Carte::gererZoom(float delta)
{
	sf::Vector2f tailleEcran(m_viewInterface.getSize()), tailleJeu(m_viewJeu.getSize());
	float ancienFacteur(tailleJeu.x / tailleEcran.x);
	
	float nouveauFacteur;

	nouveauFacteur = ancienFacteur - delta * 0.1;
	if (nouveauFacteur < 0.5)
	{
		nouveauFacteur = 0.5;
	}
	else if (nouveauFacteur > 3)
	{
		nouveauFacteur = 3;
	}

	m_viewJeu.setSize(tailleEcran.x * nouveauFacteur, tailleEcran.y * nouveauFacteur);
}


void Carte::decouvrirAsteroides()
{
	sf::Vector2f centre(m_viewJeu.getCenter());
	std::vector<std::pair<int, int>> chunksPotentiels;
	std::pair<int, int> chunkCentre((centre.x - 2500) / 5000, (centre.y - 2500) / 5000);

	chunksPotentiels.push_back(chunkCentre);
	chunksPotentiels.push_back(std::pair<int, int>(chunkCentre.first+1, chunkCentre.second+1));
	chunksPotentiels.push_back(std::pair<int, int>(chunkCentre.first+1, chunkCentre.second));
	chunksPotentiels.push_back(std::pair<int, int>(chunkCentre.first+1, chunkCentre.second-1));
	chunksPotentiels.push_back(std::pair<int, int>(chunkCentre.first, chunkCentre.second+1));
	chunksPotentiels.push_back(std::pair<int, int>(chunkCentre.first, chunkCentre.second-1));
	chunksPotentiels.push_back(std::pair<int, int>(chunkCentre.first-1, chunkCentre.second+1));
	chunksPotentiels.push_back(std::pair<int, int>(chunkCentre.first-1, chunkCentre.second));
	chunksPotentiels.push_back(std::pair<int, int>(chunkCentre.first-1, chunkCentre.second-1));

	for (int i(0); i < chunksPotentiels.size(); i++)
	{
		bool chunkDecouvert(true);

		for (int j(0); j < m_chunksDecouverts.size(); j++)
		{
			if (m_chunksDecouverts[j] == chunksPotentiels[i])
			{
				chunkDecouvert = false;
				break;
			}
		}

		if (chunkDecouvert)
		{
			m_chunksDecouverts.push_back(chunksPotentiels[i]);
			Asteroide* asteroide;
			bool pose(false);
			while (!pose)
			{
				int x(chunksPotentiels[i].first*5000 + rand() % 5000);
				int y(chunksPotentiels[i].second*5000 + rand() % 5000);

				if (x*x + y*y < 2500*2500)
				{
					continue;
				}

				int r(rand() % 300 + 100);
				asteroide = new Asteroide(r);
				asteroide->setCoord(x, y, 2*r + 100, 2*r + 100);
				for (int k(0); k < m_asteroides.size(); k++)
				{
					if (m_asteroides[k]->collisionAsteroide(asteroide))
					{
						delete asteroide;
						asteroide = 0;
						break;
					}
				}

				if (asteroide != 0)
				{
					asteroide->generer();
					m_asteroides.push_back(asteroide);
					pose = true;
				}
			}
		}
	}
}


void Carte::gererPersonnages(float dt)
{
	gererIAPersonnages();
	deplacerPersonnages(dt);
}


void Carte::gererIAPersonnages()
{
	// Regarder s'il faut activer l'IA

	bool activer(false);
	for (int i(0); i < m_personnages.size(); i++)
	{
		activer = activer || (!m_personnages[i]->estOccupe());
	}

	if (!activer)
	{
		return;
	}

	// Finir une tâche pour laquelle on s'est déplacé

	for (int i(0); i < m_personnages.size(); i++)
	{
		if (!m_personnages[i]->estOccupe())
		{
			Machine* machineCible(m_personnages[i]->getMachineCible());
			Ressource* ressourcePersonnage(m_personnages[i]->getRessource(0));
			int emplacement(m_personnages[i]->getEmplacementCible());

			if (m_personnages[i]->getAction() == Personnage::Action::DeposerRessource)
			{
				if (m_personnages[i]->getArrivee() == machineCible->getNoeudProche())
				{
					machineCible->deposerRessource(ressourcePersonnage, emplacement);
					m_personnages[i]->retirerRessource(ressourcePersonnage);
				}
				machineCible->libererEmplacement(emplacement);
			}
			else if (m_personnages[i]->getAction() == Personnage::Action::PrendreRessource)
			{
				if (m_personnages[i]->getArrivee() == machineCible->getNoeudProche())
				{
					Ressource* ressourcePrise(machineCible->getRessource(emplacement));
					m_personnages[i]->deposerRessource(ressourcePrise, 0);
					machineCible->retirerRessource(ressourcePrise);
				}
				machineCible->libererEmplacement(emplacement);
			}
		}
	}

	// Déterminer les endroits où une ressource est nécessaire/disponnible

	struct Emplacement
	{
		Machine* machine;
		int emplacement;
	};

	std::vector<std::vector<Emplacement>> ressourcesNecessaires, ressourcesDeposables, ressourcesDisponnibles, ressourcesEnTrop;
	Emplacement e;

	for (int i(0); i < Ressource::nbTypes; i++)
	{
		ressourcesNecessaires.push_back(std::vector<Emplacement>());
		ressourcesDeposables.push_back(std::vector<Emplacement>());
		ressourcesDisponnibles.push_back(std::vector<Emplacement>());
		ressourcesEnTrop.push_back(std::vector<Emplacement>());
	}

	for (int i(0); i < m_salles.size(); i++)
	{
		std::vector<Machine*>* machines(m_salles[i]->getMachines());
		for (int j(0); j < machines->size(); j++)
		{
			for (int k(0); k < (*machines)[j]->getTaille(); k++)
			{
				if ((*machines)[j]->ressourcePresente(k) && (*machines)[j]->ressourceDisponnible(k))
				{
					e.machine = (*machines)[j];
					e.emplacement = k;
					ressourcesDisponnibles[(*machines)[j]->getTypeRessource(k)].push_back(e);
				}

				if ((*machines)[j]->ressourceEnTrop(k))
				{
					e.machine = (*machines)[j];
					e.emplacement = k;
					ressourcesEnTrop[(*machines)[j]->getTypeRessource(k)].push_back(e);
				}

				for (int l(0); l < Ressource::nbTypes; l++)
				{
					Ressource ressourceTest(l);
					if ((*machines)[j]->ressourceNecessaire(&ressourceTest, k))
					{
						e.machine = (*machines)[j];
						e.emplacement = k;
						ressourcesNecessaires[l].push_back(e);
					}
					if ((*machines)[j]->ressourceDeposable(&ressourceTest, k))
					{
						e.machine = (*machines)[j];
						e.emplacement = k;
						ressourcesDeposables[l].push_back(e);
					}
				}
			}
		}
	}

	// Se déplacer vers une tâche

	for (int i(0); i < m_personnages.size(); i++)
	{
		if (!m_personnages[i]->estOccupe())
		{
			Ressource* ressourcePersonnage(m_personnages[i]->getRessource(0));
			Machine* machine;
			int emplacement;

			// Déposer sa ressource

			if (ressourcePersonnage != 0)
			{
				int type(ressourcePersonnage->getType());

				// Dans une machine qui en a besoin

				if (ressourcesNecessaires[type].size() != 0)
				{
					machine = ressourcesNecessaires[type][0].machine;
					emplacement = ressourcesNecessaires[type][0].emplacement;
					ressourcesNecessaires.erase(ressourcesNecessaires.begin());

					while (ressourcesNecessaires[type].size() != 0 && !machine->emplacementLibre(emplacement))
					{
						machine = ressourcesNecessaires[type][0].machine;
						emplacement = ressourcesNecessaires[type][0].emplacement;
						ressourcesNecessaires.erase(ressourcesNecessaires.begin());
					}

					if (machine->emplacementLibre(emplacement) && machine->ressourceNecessaire(ressourcePersonnage, emplacement))
					{
						machine->reserverEmplacement(emplacement);
						m_personnages[i]->setMachineCible(machine);
						m_personnages[i]->setEmplacementCible(emplacement);
						m_personnages[i]->ajusterTrajectoire(machine->getNoeudProche());
						m_personnages[i]->setAction(Personnage::Action::DeposerRessource);
						continue;
					}
				}

				// Dans une machine qui n'en a pas besoin

				if (!m_personnages[i]->estOccupe() && ressourcesDeposables[type].size() != 0)
				{
					machine = ressourcesDeposables[type][0].machine;
					emplacement = ressourcesDeposables[type][0].emplacement;
					ressourcesDeposables.erase(ressourcesDeposables.begin());

					while (ressourcesDeposables[type].size() != 0 && !machine->emplacementLibre(emplacement))
					{
						machine = ressourcesDeposables[type][0].machine;
						emplacement = ressourcesDeposables[type][0].emplacement;
						ressourcesDeposables.erase(ressourcesDeposables.begin());
					}

					if (machine->emplacementLibre(emplacement) && machine->ressourceDeposable(ressourcePersonnage, emplacement))
					{
						machine->reserverEmplacement(emplacement);
						m_personnages[i]->setMachineCible(machine);
						m_personnages[i]->setEmplacementCible(emplacement);
						m_personnages[i]->ajusterTrajectoire(machine->getNoeudProche());
						m_personnages[i]->setAction(Personnage::Action::DeposerRessource);
						continue;
					}
				}
			}

			// Aller prendre une ressource

			else
			{
				// Nettoyer les listes

				int type(0);
				bool typeTrouve(false);
				while (type < Ressource::nbTypes)
				{
					while (ressourcesNecessaires[type].size() != 0 && !ressourcesNecessaires[type][0].machine->emplacementLibre(ressourcesNecessaires[type][0].emplacement))
					{
						ressourcesNecessaires[type].erase(ressourcesNecessaires[type].begin());
					}
					while (ressourcesDeposables[type].size() != 0 && !ressourcesDeposables[type][0].machine->emplacementLibre(ressourcesDeposables[type][0].emplacement))
					{
						ressourcesDeposables[type].erase(ressourcesDeposables[type].begin());
					}
					while (ressourcesDisponnibles[type].size() != 0 && !ressourcesDisponnibles[type][0].machine->emplacementLibre(ressourcesDisponnibles[type][0].emplacement))
					{
						ressourcesDisponnibles[type].erase(ressourcesDisponnibles[type].begin());
					}
					while (ressourcesEnTrop[type].size() != 0 && !ressourcesEnTrop[type][0].machine->emplacementLibre(ressourcesEnTrop[type][0].emplacement))
					{
						ressourcesEnTrop[type].erase(ressourcesEnTrop[type].begin());
					}
					type++;
				}

				// S'il y a une ressource en trop qui est nécessaire ailleurs

				type = 0;
				typeTrouve = false;
				while (type < Ressource::nbTypes && !typeTrouve)
				{
					if (ressourcesEnTrop[type].size() != 0 && ressourcesNecessaires[type].size() != 0)
					{
						typeTrouve = true;
					}
					else
					{
						type++;
					}
				}

				if (typeTrouve)
				{
					machine = ressourcesEnTrop[type][0].machine;
					emplacement = ressourcesEnTrop[type][0].emplacement;
					machine->reserverEmplacement(emplacement);
					m_personnages[i]->setMachineCible(machine);
					m_personnages[i]->setEmplacementCible(emplacement);
					m_personnages[i]->ajusterTrajectoire(machine->getNoeudProche());
					m_personnages[i]->setAction(Personnage::Action::PrendreRessource);
					continue;
				}

				// S'il y a une ressource disponnible qui est nécessaire ailleurs

				type = 0;
				typeTrouve = false;
				while (type < Ressource::nbTypes && !typeTrouve)
				{
					if (ressourcesDisponnibles[type].size() != 0 && ressourcesNecessaires[type].size() != 0)
					{
						typeTrouve = true;
					}
					else
					{
						type++;
					}
				}

				if (typeTrouve)
				{
					machine = ressourcesDisponnibles[type][0].machine;
					emplacement = ressourcesDisponnibles[type][0].emplacement;
					machine->reserverEmplacement(emplacement);
					m_personnages[i]->setMachineCible(machine);
					m_personnages[i]->setEmplacementCible(emplacement);
					m_personnages[i]->ajusterTrajectoire(machine->getNoeudProche());
					m_personnages[i]->setAction(Personnage::Action::PrendreRessource);
					continue;
				}

				// S'il y a une ressource en trop qui est déposable ailleurs

				type = 0;
				typeTrouve = false;
				while (type < Ressource::nbTypes && !typeTrouve)
				{
					if (ressourcesEnTrop[type].size() != 0 && ressourcesDeposables[type].size() != 0)
					{
						typeTrouve = true;
					}
					else
					{
						type++;
					}
				}

				if (typeTrouve)
				{
					machine = ressourcesEnTrop[type][0].machine;
					emplacement = ressourcesEnTrop[type][0].emplacement;
					machine->reserverEmplacement(emplacement);
					m_personnages[i]->setMachineCible(machine);
					m_personnages[i]->setEmplacementCible(emplacement);
					m_personnages[i]->ajusterTrajectoire(machine->getNoeudProche());
					m_personnages[i]->setAction(Personnage::Action::PrendreRessource);
					continue;
				}
			}
		}

		if (!m_personnages[i]->estOccupe())
		{
			Noeud* noeudCible(0);
			int salleCible(0);

			while (noeudCible == 0)
			{
				salleCible = rand() % m_salles.size();
				noeudCible = m_salles[salleCible]->getNoeudNormal();
			}

			m_personnages[i]->ajusterTrajectoire(noeudCible);
			m_personnages[i]->setAction(Personnage::Action::Ballade);
		}
	}
}


void Carte::deplacerPersonnages(float dt)
{
	for (int i(0); i < m_personnages.size(); i++)
	{
		for (int j(0); j < m_salles.size(); j++)
		{
			if (m_salles[j]->appartenanceNoeud(m_personnages[i]->getDepart()))
			{
				m_salles[j]->deplacerPersonnage(m_personnages[i], dt);
				break;
			}

			// TODO: Gérer ici le cas getDepart == 0 créé par la destruction de la salle de départ
		}
	}
}


float Carte::energieProduite(float energie, float energieSolaire) const
{
	float heure(heureJournee() / Carte::DureeJour), pi(3.1415926);

	if (heure > 1)
	{
		if (heure < 1.1)
		{
			return energie + energieSolaire * cos((heure - 1) * 10 * pi / 2);
		}
		else if (heure < 1.9)
		{
			return energie;
		}
		else
		{
			return energie + energieSolaire * cos((2 - heure) * 10 * pi / 2);
		}
	}
	else
	{
		return energie + energieSolaire;
	}
}


float Carte::energieProduite(Salle* salle) const
{
	return energieProduite(salle->getEnergieProduite(), salle->getEnergieSolaireProduite());
}


float Carte::energieConsoMachines() const
{
	float energie(0);

	for (int i(0); i < m_salles.size(); i++)
	{
		energie += m_salles[i]->getEnergieConsoMachine();
	}

	return energie;
}


float Carte::energieConsommee(float energie, float energieMachine) const
{
	return energie + energieMachine;
}


float Carte::energieConsommee(Salle* salle) const
{
	return energieConsommee(salle->getEnergieConso(), salle->getEnergieConsoMachine());
}


void Carte::gererEnergie(float dt)
{
	// Déterminer l'énergie de chaque salle, trouver les salles sources et accu

	std::vector<Salle*> sourcesElec, accusElec;
	std::unordered_map<Salle*, float> valeurElec;
	std::unordered_map<Salle*, bool> marquee;

	for (int i(0); i < m_salles.size(); i++)
	{
		m_salles[i]->setPanneElectrique(false);
		valeurElec[m_salles[i]] = energieProduite(m_salles[i]) - energieConsommee(m_salles[i]);

		if (valeurElec[m_salles[i]] > 0)
		{
			sourcesElec.push_back(m_salles[i]);
		}
		if (m_salles[i]->getCapaciteAccu() > 0)
		{
			accusElec.push_back(m_salles[i]);
		}
	}

	// Redistribuer l'énergie dans les salles

	for (int i(0); i < sourcesElec.size(); i++)
	{
		for (int j(0); j < m_salles.size(); j++)
		{
			marquee[m_salles[j]] = false;
		}

		std::queue<Salle*> sallesDisponnibles;
		sallesDisponnibles.push(sourcesElec[i]);
		Salle* salleEnCours(0);
		float energie(valeurElec[sourcesElec[i]]);

		while (energie > 0 && sallesDisponnibles.size() > 0)
		{
			salleEnCours = sallesDisponnibles.front();
			sallesDisponnibles.pop();

			for (int j(0); j < salleEnCours->getVoisines()->size(); j++)
			{
				Salle* voisine((*salleEnCours->getVoisines())[j]);
				if (!marquee[voisine])
				{
					sallesDisponnibles.push(voisine);
					marquee[voisine] = true;
				}
				if (valeurElec[voisine] < 0)
				{
					if (valeurElec[voisine] + energie > 0)
					{
						energie += valeurElec[voisine];
						valeurElec[voisine] = 0;
					}
					else
					{
						valeurElec[voisine] += energie;
						energie = 0;
						break;
					}
				}
			}
		}

		valeurElec[sourcesElec[i]] = energie;

		if (energie > 0)
		{
			break;
		}
	}

	// Calculer l'énergie restante à distribuer en utilisant les accus

	float energieRestante(0);
	for (int i(0); i < m_salles.size(); i++)
	{
		energieRestante += valeurElec[m_salles[i]];
	}

	if (energieRestante == 0)
	{
		return;
	}

	// Redistribuer l'énergie en utilisant les accus

	if (energieRestante < 0)
	{
		energieRestante *= -dt;
		for (int i(0); i < accusElec.size(); i++)
		{
			float energieAccu(accusElec[i]->getEnergieAccu());
			if (energieAccu > energieRestante)
			{
				accusElec[i]->setEnergieAccu(energieAccu - energieRestante);
				energieRestante = 0;
				break;
			}
			else
			{
				energieRestante -= energieAccu;
				accusElec[i]->setEnergieAccu(0);
			}
		}

		if (energieRestante != 0)
		{
			for (int i(0); i < m_salles.size(); i++)
			{
				if (valeurElec[m_salles[i]] < 0)
				{
					m_salles[i]->setPanneElectrique(true);
				}
			}
		}
	}
	else
	{
		energieRestante *= dt;

		for (int i(0); i < accusElec.size(); i++)
		{
			float energieAccu(accusElec[i]->getEnergieAccu()), capaciteAccu(accusElec[i]->getCapaciteAccu());
			if (capaciteAccu - energieAccu > energieRestante)
			{
				accusElec[i]->setEnergieAccu(energieAccu + energieRestante);
				break;
			}
			else
			{
				accusElec[i]->setEnergieAccu(capaciteAccu);
				energieRestante -= capaciteAccu - energieAccu;
			}
		}
	}

	// Recalculer l'énergie totale accumulée

	m_energieAccu = 0;
	for (int i(0); i < accusElec.size(); i++)
	{
		m_energieAccu += accusElec[i]->getEnergieAccu();
	}
}


float Carte::chaleurDissipeeMachines() const
{
	float chaleur(0);

	for (int i(0); i < m_salles.size(); i++)
	{
		chaleur += m_salles[i]->getChaleurDissipeeMachines();
	}

	return chaleur;
}


void Carte::gererTemperature(float dt)
{
	m_chaleurDissipee = 0;
	for (int i(0); i < m_salles.size(); i++)
	{
		if (!m_salles[i]->getPanneElectrique())
		{
			m_chaleurDissipee += m_salles[i]->getChaleurDissipee();
		}
	}

	float temperature(15);
	float chaleurDissipee(m_chaleurDissipee + chaleurDissipeeMachines());
	float energieConso(m_energieConso + energieConsoMachines());

	if (energieConso > chaleurDissipee)
	{
		temperature += (energieConso - chaleurDissipee);
	}

	if (temperature != m_temperature)
	{
		m_temperature += dt * (temperature - m_temperature) / 2;
		if (!(m_temperature - temperature > 0.01f || temperature - m_temperature > 0.01f))
		{
			m_temperature = temperature;
		}
	}
}


void Carte::gererMachinesRessources(float dt)
{
	for (int i(0); i < m_salles.size(); i++)
	{
		m_salles[i]->faireFonctionner(dt);
	}

	for (int i(0); i < m_ressources.size(); i++)
	{
		if (m_ressources[i]->getEtat() == 0)
		{
			delete m_ressources[i];
			m_ressources.erase(m_ressources.begin() + i);
			i--;
		}
	}
}


void Carte::gererClicMenu(sf::RenderWindow const& window, std::vector<Salle*>& sallesPointees, Machine* machinePointee, Salle*& nouvelleSalle, Station*& nouvelleStation)
{
	// Replier des menus avant ce clic

	m_menus[0]->setAfficher(sallesPointees.size() == 0 && machinePointee == 0);
	m_menus[3]->setAfficher(sallesPointees.size() != 0);
	m_menus[5]->setAfficher(machinePointee != 0);

	// Obtenir l'emplacement du clic

	int indiceMenu(-1), indiceBouton(-1);

	for (int i(0); i < m_menus.size(); i++)
	{
		int j(m_menus[i]->getClicBouton(window));
		if (j != -1)
		{
			indiceMenu = i;
			indiceBouton = j;
		}
	}

	// Replier des menus après ce clic

	if (!(indiceMenu == 0 && indiceBouton == 1))
	{
		m_menus[2]->setAfficher(false);
	}

	if (!(indiceMenu == 0 && indiceBouton == 2))
	{
		m_menus[6]->setAfficher(false);
	}

	// Gérer le clic

	if (indiceMenu == 0)  // HUD gauche
	{
		if (indiceBouton == 0)
		{
			
		}
		else if (indiceBouton == 1)
		{
			m_menus[2]->setAfficher(!m_menus[2]->estAffiche());
		}
		else if (indiceBouton == 2)
		{
			m_menus[6]->setAfficher(!m_menus[6]->estAffiche());
		}
	}

	else if (indiceMenu == 1)  // HUD droit
	{

	}

	else if (indiceMenu == 2)  // Creation de salle
	{
		switch (indiceBouton)
		{
			case 0:
				nouvelleSalle = new CouloirI(m_loader);
				break;
			case 1:
				nouvelleSalle = new CouloirT(m_loader);
				break;
			case 2:
				nouvelleSalle = new CouloirL(m_loader);
				break;
			case 3:
				nouvelleSalle = new CouloirX(m_loader);
				break;
			case 4:
				nouvelleSalle = new SalleDeBatteries(m_loader);
				break;
			case 5:
				nouvelleSalle = new CentraleElectrique(m_loader);
				break;
			case 6:
				nouvelleSalle = new PanneauSolaire(m_loader);
				break;
			case 7:
				nouvelleSalle = new PileRadioIsotopique(m_loader);
				break;
			case 8:
				nouvelleSalle = new ReserveEau(m_loader);
				break;
			case 9:
				nouvelleSalle = new SalleTraitementEau(m_loader);
				break;
			case 10:
				nouvelleSalle = new SalleDeRefroidissement(m_loader);
				break;
			case 11:
				nouvelleSalle = new PanneauRadiateur(m_loader);
				break;
			case 12:
				nouvelleSalle = new SAS(m_loader);
				break;
			case 13:
				nouvelleSalle = new Reserve(m_loader);
				break;
			case 14:
				nouvelleSalle = new Fonderie(m_loader);
				break;
			default:
				break;
		}
	}

	else if (indiceMenu == 3)  // Detail salle
	{
		if (indiceBouton == 0)
		{
			if (enleverSalles(sallesPointees))
			{
				sallesPointees.clear();

				m_menus[0]->setAfficher(true);
				m_menus[3]->setAfficher(false);
			}
		}
	}

	else if (indiceMenu == 4)  // Fusion salles
	{
		if (!sallesPointees[0]->raccorderSalle(*(sallesPointees[1])))
		{
			ajouterInformation("Ces salles ne peuvent pas être raccordées.");
		}
	}

	else if (indiceMenu == 5) // Détails machine
	{

	}

	else if (indiceMenu == 6)  // Creation station
	{
		switch (indiceBouton)
		{
			case 0:
				nouvelleStation = new Foreuse(m_loader);
				break;
			default:
				break;
		}
	}
}


void Carte::ajouterInformation(std::string chaine)
{
	sf::Vector2f tailleView(m_viewInterface.getSize());
	sf::VideoMode resolution(tailleView.x, tailleView.y);
	FenetreInformation fenetre(m_loader, resolution, chaine);

	if (m_fenetresInformation.size() < 5)
	{
		m_fenetresInformation.push_back(fenetre);
	}
	else
	{
		for (int i(1); i < 5; i++)
		{
			m_fenetresInformation[i - 1] = m_fenetresInformation[i];
		}
		m_fenetresInformation[4] = fenetre;
	}
}


void Carte::gestionInformation(sf::RenderWindow const& window)
{
	for (int i(0); i < m_fenetresInformation.size(); i++)
	{

		if (m_fenetresInformation[i].estActive(window))
		{
			if (i == 0)
			{
				m_fenetresInformation[i].ajusterDecalage();
			}
			else
			{
				m_fenetresInformation[i].ajusterDecalage(m_fenetresInformation[i - 1]);
			}
		}
		else
		{
			m_fenetresInformation.erase(m_fenetresInformation.begin() + i);
			i--;
		}
	}
}


bool Carte::collisionSourisMenu(sf::RenderWindow const& window)
{
	for (int i(0); i < m_menus.size(); i++)
	{
		if (m_menus[i]->estAffiche() && m_menus[i]->collisionSouris(window))
		{
			return true;
		}
	}

	return false;
}


bool Carte::collisionSourisMachine(sf::RenderWindow const& window)
{
	int decalageX, decalageY;
	decalageX = m_viewJeu.getCenter().x - m_viewJeu.getSize().x / 2;
	decalageY = m_viewJeu.getCenter().y - m_viewJeu.getSize().y / 2;
	float facteur(m_viewJeu.getSize().x / m_viewInterface.getSize().x);

	for (int i(0); i < m_salles.size(); i++)
	{
		std::vector<Machine*>* machines(m_salles[i]->getMachines());

		for (int j(0); j < machines->size(); j++)
		{
			Entite machine((m_salles[i]->getX() + (*machines)[j]->getX() - decalageX) / facteur,
						   (m_salles[i]->getY() + (*machines)[j]->getY() - decalageY) / facteur,
						   (*machines)[j]->getW() / facteur,
						   (*machines)[j]->getH() / facteur);

			if (machine.collisionSouris(window))
			{
				return true;
			}
		}
	}

	return false;
}


Salle* Carte::preparerCreationSalle(sf::RenderWindow& window, Salle* nouvelleSalle)
{
	if (nouvelleSalle == 0)
	{
		return 0;
	}
	
	// Déterminer si une salle connectable est proche et si oui, déplacer la nouvelle salle et marquer la salle connectable

	sf::Vector2i positionSouris(sf::Mouse::getPosition(window));
	sf::Vector2f centreView(m_viewJeu.getCenter()), tailleView(m_viewJeu.getSize());
	float facteur(m_viewJeu.getSize().x / m_viewInterface.getSize().x);

	nouvelleSalle->setCoord(positionSouris.x * facteur - nouvelleSalle->getW() / 2 + centreView.x - tailleView.x / 2, positionSouris.y * facteur - nouvelleSalle->getH() / 2 + centreView.y - tailleView.y / 2);

	int salleConnectable(-1);

	for (int i(0); i < m_salles.size(); i++)
	{
		if (m_salles[i]->essayerConnecterSalle(*nouvelleSalle))
		{
			salleConnectable = i;
			break;
		}
	}

	// Afficher la nouvelle salle

	std::vector<Connexion>* connexionsSalle((nouvelleSalle->getConnexions()));

	window.setView(m_viewJeu);

	std::vector<int> flags;
	flags.push_back(Salle::FlagsAffichage::Creation);
	nouvelleSalle->afficher(m_loader, window, flags, 15, 0);

	if (salleConnectable != -1)
	{
		for (int i(0); i < connexionsSalle->size(); i++)
		{
			if ((*connexionsSalle)[i].estConnectee())
			{
				(*connexionsSalle)[i].deconnecter();
				sf::RectangleShape spriteConnexion(sf::Vector2f((*connexionsSalle)[i].getW(), (*connexionsSalle)[i].getH()));
				spriteConnexion.setFillColor(sf::Color(0, 255, 0, 100));
				spriteConnexion.setPosition(nouvelleSalle->getX() + (*connexionsSalle)[i].getX(), nouvelleSalle->getY() + (*connexionsSalle)[i].getY());

				window.draw(spriteConnexion);
			}
		}
	}

	window.setView(m_viewInterface);

	// Déterminer si la nouvelle salle est en collision avec d'autres salles et si c'est le cas, annuler la connectabilité

	for (int i(0); i < m_salles.size(); i++)
	{
		if (i != salleConnectable && nouvelleSalle->collision(*m_salles[i]))
		{
			salleConnectable = -1;
		}
	}

	// Déterminer si la nouvelle salle est en collision avec un asteroide et si c'est le cas, annuler la connectabilité

	for (int i(0); i < m_asteroides.size(); i++)
	{
		if (nouvelleSalle->collision(*m_asteroides[i]))
		{
			salleConnectable = -1;
		}
	}

	// Retourner la salle connectable

	if (salleConnectable != -1)
	{
		return m_salles[salleConnectable];
	}
	else
	{
		return 0;
	}
}


Asteroide* Carte::preparerCreationStation(sf::RenderWindow& window, Station* nouvelleStation)
{
	if (nouvelleStation == 0)
	{
		return 0;
	}

	// Déterminer si un asteroide est proche et si oui, déplacer la nouvelle station

	sf::Vector2i positionSouris(sf::Mouse::getPosition(window));
	sf::Vector2f centreView(m_viewJeu.getCenter()), tailleView(m_viewJeu.getSize());
	float facteur(m_viewJeu.getSize().x / m_viewInterface.getSize().x);

	nouvelleStation->setCoord(positionSouris.x*facteur - nouvelleStation->getW()/2 + centreView.x - tailleView.x/2, positionSouris.y*facteur - nouvelleStation->getH()/2 + centreView.y - tailleView.y/2);

	int asteroideConnectable(-1);
	float angle(0), diametre(0);
	for (int i(0); i < m_asteroides.size(); i++)
	{
		int rayon(m_asteroides[i]->getRayon() + 50);
		float deltaX(nouvelleStation->getX() + nouvelleStation->getW()/2 - m_asteroides[i]->getX() - rayon);
		float deltaY(m_asteroides[i]->getY() - nouvelleStation->getY() - nouvelleStation->getH()/2 + rayon);
		float dist(sqrt(pow(deltaX, 2) + pow(deltaY, 2)));
		if (dist > rayon - 99 && dist < rayon + 99)
		{
			angle = atan2(deltaY, deltaX) - 1.570796;
			diametre = 2 * atan(float(nouvelleStation->getW())/(2*rayon));
			nouvelleStation->setAngle(angle);
			nouvelleStation->setDiametreAngulaire(diametre);
			nouvelleStation->setCoord(m_asteroides[i]->getX() + rayon - rayon*sin(angle) - nouvelleStation->getW()/2, m_asteroides[i]->getY() + rayon - rayon*cos(angle) - nouvelleStation->getH()/2);
			asteroideConnectable = i;
			break;
		}
	}

	// Annuler la connectabilité si la station est en collision avec d'autres stations

	int collision(0);
	if (asteroideConnectable != -1)
	{
		collision = -1;
		std::vector<Machine*>* machines(m_asteroides[asteroideConnectable]->getMachines());
		for (int i(0); i < machines->size(); i++)
		{
			if (nouvelleStation->collisionAngulaire((Station*) (*machines)[i]))
			{
				collision = 1;
				asteroideConnectable = -1;
				break;
			}
		}
	}

	// Afficher la nouvelle station

	window.setView(m_viewJeu);

	sf::Texture* texture(nouvelleStation->getTexture());
	sf::Sprite sprite(*texture);
	if (collision == 0)
	{
		sprite.setColor(sf::Color(255, 255, 255, 127));
	}
	else if (collision == 1)
	{
		sprite.setColor(sf::Color(255, 127, 127, 127));
	}
	else
	{
		sprite.setColor(sf::Color(127, 255, 127, 127));
	}
	sprite.setOrigin(nouvelleStation->getW() / 2, nouvelleStation->getH() / 2);
	sprite.setPosition(nouvelleStation->getX() + nouvelleStation->getW() / 2, nouvelleStation->getY() + nouvelleStation->getH() / 2);
	sprite.setRotation(-angle * 57.3);
	window.draw(sprite);

	window.setView(m_viewInterface);

	// Retourner la salle connectable

	if (asteroideConnectable != -1)
	{
		return m_asteroides[asteroideConnectable];
	}
	else
	{
		return 0;
	}
}


void Carte::ajouterSalle(Salle* nouvelleSalle, Salle* salleConnectee)
{
	m_salles.push_back(nouvelleSalle);
	salleConnectee->connecterSalle(*nouvelleSalle);

	m_energieConso += nouvelleSalle->getEnergieConso();
	m_energieProduite += nouvelleSalle->getEnergieProduite();
	m_energieSolaireProduite += nouvelleSalle->getEnergieSolaireProduite();
	m_capaciteAccu += nouvelleSalle->getCapaciteAccu();
	m_energieAccu += nouvelleSalle->getEnergieAccu();
	m_chaleurDissipee += nouvelleSalle->getChaleurDissipee();
}


void Carte::ajouterStation(Station* nouvelleStation, Asteroide* asteroideProche)
{
	nouvelleStation->setCoord(nouvelleStation->getX() - asteroideProche->getX(), nouvelleStation->getY() - asteroideProche->getY());
	asteroideProche->connecterStation(nouvelleStation);

	for (int i(0); i < m_salles.size(); i++)
	{
		if (m_salles[i] == asteroideProche)
		{
			return;
		}
	}

	m_salles.push_back(asteroideProche);
}


bool Carte::enleverSalles(std::vector<Salle*>& sallesPointees)
{
	if (sallesPointees.size() == 0)
	{
		return false;
	}

	if (emplacementSelection(sallesPointees, m_salles[0]) != -1)
	{
		ajouterInformation("La salle de commandes ne peut pas être supprimée.");
		return false;
	}

	// Tester s'il est possible de supprimer l'ensemble des salles pointees

	Salle* salleEnCours(0);
	std::vector<Salle*> sallesMarquees;
	std::queue<Salle*> sallesDisponnibles;
	sallesDisponnibles.push(m_salles[0]);
	sallesMarquees.push_back(m_salles[0]);

	while (sallesDisponnibles.size() != 0)
	{
		salleEnCours = sallesDisponnibles.front();
		sallesDisponnibles.pop();

		for (int i(0); i < salleEnCours->getVoisines()->size(); i++)
		{
			Salle* salleVoisine((*salleEnCours->getVoisines())[i]);
			if (emplacementSelection(sallesMarquees, salleVoisine) == -1 && emplacementSelection(sallesPointees, salleVoisine) == -1)
			{
				sallesMarquees.push_back(salleVoisine);
				sallesDisponnibles.push(salleVoisine);
			}
		}
	}

	if (sallesMarquees.size() + sallesPointees.size() != m_salles.size())
	{
		ajouterInformation("Il est impossible de supprimer cette/ces salle(s).\nDes salles seraient déconnectées de la salle des commandes.");
		return false;
	}

	// Détruire l'ensemble des salles

	for (int i(0); i < sallesPointees.size(); i++)
	{
		// Gérer les personnages qui sont ou vont dans la salle

		for (int j(0); j < m_personnages.size(); j++)
		{
			if (sallesPointees[i]->contientPersonnage(m_personnages[j]))
			{
				delete m_personnages[j];
				m_personnages.erase(m_personnages.begin() + j);
				j--;
			}
			else if (sallesPointees[i]->estTraversee(m_personnages[j]))
			{
				m_personnages[j]->annulerTrajectoire();
			}
		}

		// Supprimer la salle

		int k(-1);

		for (int j(0); j < m_salles.size(); j++)
		{
			if (m_salles[j] == sallesPointees[i])
			{
				k = j;
			}
		}

		m_energieConso -= m_salles[k]->getEnergieConso();
		m_energieProduite -= m_salles[k]->getEnergieProduite();
		m_energieSolaireProduite -= m_salles[k]->getEnergieSolaireProduite();
		m_capaciteAccu -= m_salles[k]->getCapaciteAccu();
		m_energieAccu -= m_salles[k]->getEnergieAccu();
		m_chaleurDissipee -= m_salles[k]->getChaleurDissipee();

		m_salles.erase(m_salles.begin() + k);
		delete sallesPointees[i];
	}

	return true;
}


void Carte::assurerConnectionSAS()
{
	for (int i(0); i < m_asteroides.size(); i++)
	{
		if (m_asteroides[i]->estConnecteSalles())
		{
			m_asteroides[i]->assurerConnectionSAS();
		}
	}
}


void Carte::selectionSalle(sf::RenderWindow const& window, std::vector<Salle*>& sallesPointees) const
{
	const bool shiftPresse(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift));

	int decalageX, decalageY;
	decalageX = m_viewJeu.getCenter().x - m_viewJeu.getSize().x / 2;
	decalageY = m_viewJeu.getCenter().y - m_viewJeu.getSize().y / 2;

	if (m_menus[3]->estAffiche() && m_menus[3]->collisionSouris(window))
	{
		return;
	}

	float facteur(m_viewJeu.getSize().x / m_viewInterface.getSize().x);
	for (int i(0); i < m_salles.size(); i++)
	{
		Entite salle((m_salles[i]->getX() - decalageX) / facteur, (m_salles[i]->getY() - decalageY) / facteur, m_salles[i]->getW() / facteur, m_salles[i]->getH() / facteur);

		if (salle.collisionSouris(window))
		{
			if (shiftPresse)
			{
				int emplacement(emplacementSelection(sallesPointees, m_salles[i]));

				if (emplacement == -1)
				{
					sallesPointees.push_back(m_salles[i]);
				}
				else
				{
					sallesPointees.erase(sallesPointees.begin() + emplacement);
				}
			}
			else
			{
				sallesPointees.clear();
				sallesPointees.push_back(m_salles[i]);
				return;
			}
		}
	}

	if (!shiftPresse)
	{
		sallesPointees.clear();
	}
}


void Carte::selectionMachine(sf::RenderWindow const& window, Machine*& machinePointee) const
{
	int decalageX, decalageY;
	decalageX = m_viewJeu.getCenter().x - m_viewJeu.getSize().x / 2;
	decalageY = m_viewJeu.getCenter().y - m_viewJeu.getSize().y / 2;
	float facteur(m_viewJeu.getSize().x / m_viewInterface.getSize().x);

	for (int i(0); i < m_salles.size(); i++)
	{
		std::vector<Machine*>* machines(m_salles[i]->getMachines());

		for (int j(0); j < machines->size(); j++)
		{
			Entite machine((m_salles[i]->getX() + (*machines)[j]->getX() - decalageX) / facteur,
				(m_salles[i]->getY() + (*machines)[j]->getY() - decalageY) / facteur,
				(*machines)[j]->getW() / facteur,
				(*machines)[j]->getH() / facteur);

			if (machine.collisionSouris(window))
			{
				machinePointee = (*machines)[j];
			}
		}
	}
}


Loader* Carte::getLoader()
{
	return &m_loader;
}


sf::View* Carte::getView()
{
	return &m_viewJeu;
}


std::vector<Menu*>* Carte::getMenus()
{
	return &m_menus;
}


Carte::~Carte()
{
	for (int i(0); i < m_salles.size(); i++)
	{
		delete m_salles[i];
	}

	for (int i(0); i < m_menus.size(); i++)
	{
		delete m_menus[i];
	}
}
