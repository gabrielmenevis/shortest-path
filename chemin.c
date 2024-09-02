/*  ////////////////////////////////////////////////////
   |            LE CHEMIN LE PLUS COURT               //
   |                                                  //
   | Version : 1.0                                    //
   | Auteurs : Valentin Rivière et Gabriel Ménévis    //
   | Date : 06/02/2023                                //
   |                                                  //
   |                                                  //
   | Programme écrit pour le cours de langage C du    //
   | master MIMO (Université Paris 1) dispensé par    //
   | Jean-Michel Léry.                                //
   |                                                  //
   | Implémentation de l'algorithme de Dijkstra sur   //
   | des listes de voies, de croisements et de        //
   | segments de voies correspondant au plan du 13e   //
   | arrondissement de Paris.                         //
   |                                                  //
    --------------------------------------------------/
*/


#include "declarations.h"
#include "affichage.h"
#include "chargement.h"
#include "consultation.h"
#include "itineraire.h"



/* ##################################
   VARIABLES GLOBALES
   ##################################
*/

/* quantités de données chargées */
int nb_arcs;
int nb_voies;
int nb_noeuds;

/* pointeurs vers les premiers et derniers éléments des listes chaînées globales de voies et noeuds */
struct voie *premiere_voie, *derniere_voie;
struct noeud *premier_noeud, *dernier_noeud;



/* ##################################
   PROGRAMME PRINCIPAL
   ##################################
*/

int main()
{
	int chargement_reussi;
	int choix = -1;
	int continuer = 1;
	int saisie_correcte;
	
	/* bienvenue ! */
	printf("\n\n");
	printf("--------------------------------------------------------------\n");
	printf("|                   LE CHEMIN LE PLUS COURT                  |\n");
	printf("|        --------------------------------------------        |\n");
	printf("|                                                            |\n");
	printf("| Programme créé par Valentin Rivière et Gabriel Ménévis     |\n");
	printf("| Demandez l'itinéraire le plus court entre deux points du   |\n");
	printf("| 13e arrondissement de Paris.                               |\n");
	printf("| Vous pouvez aussi charger votre propre jeu de données dans |\n");
	printf("| le programme.                                              |\n");
	printf("--------------------------------------------------------------\n");
	printf("\n\n");
	
	/* chargement des données et récupération des erreurs éventuelles */
	chargement_reussi = chargement_global();
	
	/* le menu principal se lance si le chargement est réussi */
	if(chargement_reussi)
	{
		/* message informatif et en-tête du menu */
		printf("\n\nFin du chargement des données.\n");
		printf("--------------------------------------------------------------\n\n");

		affichage_encadre("MENU", "PRINCIPAL");
		
		/* boucle du menu principal tant que l'utilisateur ne quitte pas le programme */
		while(continuer)
		{
			printf("Que voulez-vous faire ?\n");
			printf("--------------------------------------------------------------\n");
			printf("\t\t1 - Consulter les données chargées\n");
			printf("\t\t2 - Calculer un itinéraire\n");
 			printf("\t\t3 - Charger un autre jeu de données\n");
			printf("\t\t0 - Quitter\n");
			
			/* saisie et vidage du buffer */
			printf("Votre choix : ");
			saisie_correcte = scanf("%d", &choix);
			while(getchar() != '\n');

			/* si l'utilisateur n'a pas saisi un entier on affecte une valeur invalide à choix */
			if(!saisie_correcte)
			{
				choix = -1;
			}
			
			switch(choix)
			{
				/* consultation des informations */
				case 1:
					menu_consultation();
					printf("\n\n\n");
					/* on affiche l'en-tête du menu avant de reboucler */
					affichage_encadre("MENU", "PRINCIPAL");
					break;
					
				/* calcul d'itinéraire */
				case 2:
					menu_itineraire();
					printf("\n\n\n");
					affichage_encadre("MENU", "PRINCIPAL");
					break;
					
				/* chargement de nouveaux fichiers */
				case 3:
					chargement_reussi = chargement_global();
					printf("\n\n\n");

					/* si le chargement des nouvelles données a échoué, il faut quitter le programme pour éviter toute erreur */
					if(!chargement_reussi)
					{
						continuer = 0;
					}
					else
					{
						affichage_encadre("MENU", "PRINCIPAL");
					}
					break;
					
				/* l'utilisateur quitte le programme */
				case 0:
					continuer = 0;
					liberer_memoire();
					printf("\n\n--------------------------------------------------------------\n\n");
					printf("Au revoir\n\n");
					break;
					
				/* l'utilisateur saisit une valeur non proposée */
				default:
					printf("Choix invalide.\n\n");
					break;
			}
		}
	}
}


