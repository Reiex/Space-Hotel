# Règles générales

Space-Hotel: obj/main.o obj/Carte.o obj/Menu.o obj/Salle.o obj/Asteroide.o obj/Connexion.o obj/Ressources.o obj/Personnage.o obj/Entite.o obj/fonctions.o obj/Loader.o
	g++ obj/main.o obj/Carte.o obj/Menu.o obj/Salle.o obj/Asteroide.o obj/Connexion.o obj/Ressources.o obj/Personnage.o obj/Entite.o obj/fonctions.o obj/Loader.o -o Space-Hotel -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm obj/*
	rm Space-Hotel

lineCount:
	git ls-files | xargs wc -l


# Règles de compilations de fichiers spécifiques

obj/main.o: src/main.cpp
	g++ -c src/main.cpp -o obj/main.o

obj/Carte.o: src/Carte.cpp
	g++ -c src/Carte.cpp -o obj/Carte.o

obj/Menu.o: src/Menu.cpp
	g++ -c src/Menu.cpp -o obj/Menu.o

obj/Salle.o: src/Salle.cpp
	g++ -c src/Salle.cpp -o obj/Salle.o

obj/Asteroide.o: src/Asteroide.cpp
	g++ -c src/Asteroide.cpp -o obj/Asteroide.o

obj/Connexion.o: src/Connexion.cpp
	g++ -c src/Connexion.cpp -o obj/Connexion.o

obj/Ressources.o: src/Ressources.cpp
	g++ -c src/Ressources.cpp -o obj/Ressources.o

obj/Personnage.o: src/Personnage.cpp
	g++ -c src/Personnage.cpp -o obj/Personnage.o

obj/Entite.o: src/Entite.cpp
	g++ -c src/Entite.cpp -o obj/Entite.o

obj/fonctions.o: src/fonctions.cpp
	g++ -c src/fonctions.cpp -o obj/fonctions.o

obj/Loader.o: src/Loader.cpp
	g++ -c src/Loader.cpp -o obj/Loader.o
