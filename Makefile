chemin: chemin.o affichage.o chargement.o consultation.o itineraire.o choix.o acces.o listes.o
	gcc -o chemin chemin.o affichage.o chargement.o consultation.o itineraire.o choix.o acces.o listes.o

chemin.o: chemin.c declarations.h affichage.h chargement.h consultation.h itineraire.h
	gcc -c chemin.c

chargement.o: chargement.c declarations.h chargement.h affichage.h acces.h
	gcc -c chargement.c

consultation.o: consultation.c declarations.h consultation.h affichage.h choix.h
	gcc -c consultation.c

itineraire.o: itineraire.c declarations.h itineraire.h affichage.h choix.h
	gcc -c itineraire.c

choix.o: choix.c declarations.h choix.h acces.h affichage.h listes.h
	gcc -c choix.c

acces.o: acces.c declarations.h acces.h
	gcc -c acces.c

affichage.o: affichage.c declarations.h affichage.h
	gcc -c affichage.c

listes.o: listes.c declarations.h listes.h
	gcc -c listes.c