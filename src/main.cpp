#include "Carte.h"


int main()
{
    // OUVERTURE DE LA FENÊTRE

    sf::VideoMode const resolution(sf::VideoMode::getDesktopMode());

    sf::RenderWindow window(resolution, "Space Hotel", sf::Style::Fullscreen);

    // INITIALISATION DE VARIABLES

    sf::Clock chrono;
    float dt(1/144);
	Carte carte(resolution);
	Salle *nouvelleSalle(0), *salleProcheConnexion(0);
	std::vector<Salle*> sallesPointees;
	Machine* machinePointee(0);

	std::unordered_map<std::string, sf::Keyboard::Key> raccourcis;
	raccourcis["menu options"] = sf::Keyboard::Escape;
	raccourcis["rotation"] = sf::Keyboard::R;
	raccourcis["suppression"] = sf::Keyboard::Delete;
	raccourcis["reset view"] = sf::Keyboard::Space;

    // BOUCLE DU PROGRAMME

    sf::Event event;
    bool continuerJeu(true);

    while(continuerJeu)
    {

        // MISE A JOUR DE LA FENETRE

        window.display();

        dt = (chrono.getElapsedTime().asSeconds() + 99*dt)/100;
        chrono.restart();

        // GESTION DES EVENEMENTS

        while(window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:

					continuerJeu = false;
                    break;

                case sf::Event::KeyPressed:

					if(event.key.code == raccourcis["menu options"])
					{
						continuerJeu = false;
					}
					else if (event.key.code == raccourcis["reset view"])
					{
						carte.resetView();
					}
					else if (event.key.code == raccourcis["rotation"] && nouvelleSalle != 0)
					{
						nouvelleSalle->effectuerRotation(*carte.getLoader());
					}
					else if (event.key.code == raccourcis["suppression"] && sallesPointees.size() != 0)
					{
						if (carte.enleverSalles(sallesPointees))
						{
							sallesPointees.clear();

							(*(carte.getMenus()))[0]->setAfficher(true);
							(*(carte.getMenus()))[3]->setAfficher(false);
						}
					}
					break;

				case sf::Event::MouseWheelScrolled:
					carte.gererZoom(event.mouseWheelScroll.delta);
					break;

				case sf::Event::MouseButtonReleased:

					if (event.mouseButton.button == sf::Mouse::Left)
					{
						// Création d'une nouvelle salle

						if (nouvelleSalle != 0 && salleProcheConnexion != 0)
						{
							carte.ajouterSalle(nouvelleSalle, salleProcheConnexion);
							nouvelleSalle = 0;
							salleProcheConnexion = 0;
						}

						else if (nouvelleSalle == 0)
						{
							bool menuClique(carte.collisionSourisMenu(window));
							bool machineClique(carte.collisionSourisMachine(window));

							// Selection d'une machine

							if (machineClique && !menuClique)
							{
								sallesPointees.clear();
								carte.selectionMachine(window, machinePointee);
							}

							// Selection d'une salle

							else if (!menuClique)
							{
								machinePointee = 0;
								carte.selectionSalle(window, sallesPointees);
							}

							carte.gererClicMenu(window, sallesPointees, nouvelleSalle);
						}
					}
					else if (event.mouseButton.button == sf::Mouse::Right)
					{
						// Le clic droit sert toujours à deselectionner

						if (nouvelleSalle != 0)
						{
							delete nouvelleSalle;
						}
						nouvelleSalle = 0;
						salleProcheConnexion = 0;
						sallesPointees.clear();
						machinePointee = 0;
					}
					break;

                default:
                    break;
            }
        }

		// GESTION DU JEU

		carte.avancerTemps(dt);
		carte.deplacerView(window, dt);
		carte.gestionInformation(window);
		carte.gererEnergie(dt);
		carte.gererTemperature(dt);
		carte.gererPersonnages(dt);
		carte.gererMachinesRessources(dt);

		// AFFICHAGE

		carte.afficher(window, sallesPointees, machinePointee);
		salleProcheConnexion = carte.preparerCreationSalle(window, nouvelleSalle);

		afficherFps(window, carte.getLoader(), dt);
    }

    window.close();

    return 0;
}
