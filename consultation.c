/* #######################################################
        SOUS-PROGRAMMES DE CONSULTATION DES DONNÉES
   #######################################################
*/



#include "declarations.h"
#include "consultation.h"
#include "affichage.h"
#include "choix.h"



/* déclarations de procédures locales */
void menu_voies();
void menu_noeuds();
void affichage_infos_noeud(struct noeud *a_afficher);



/* variables globales déclarées dans le fichier source principal */
extern struct voie *premiere_voie;
extern struct noeud *premier_noeud;
extern int nb_voies, nb_noeuds;




/* menu pour la consultation des informations sur les données chargées
il permet de choisir entre l'affichage des infos sur les voies : appel à menu_voies()
et sur les noeuds : appel à menu_noeuds() */
void menu_consultation()
{
	int choix;
	int continuer = 1;
	int saisie_correcte;

	/* en-tête du menu */
	affichage_encadre("MENU", "DE CONSULTATION");
	
	while(continuer)
	{
		printf("Quelles informations afficher ?\n");
		printf("--------------------------------------------------------------\n");
		printf("\t\t1 - Liste des voies\n");
		printf("\t\t2 - Liste des noeuds\n");
		printf("\t\t0 - Retour au menu principal\n");
		
		/* saisie, on vide le buffer et on contrôle la saisie */
		printf("Votre choix : ");
		saisie_correcte = scanf("%d", &choix);
		while(getchar() != '\n');

		if(!saisie_correcte)
		{
			choix = -1;
		}
		
		switch(choix)
		{
			case 1:
				menu_voies();
				printf("\n\n\n");
				affichage_encadre("MENU", "DE CONSULTATION");
				break;
				
			case 2:
				menu_noeuds();
				printf("\n\n\n");
				affichage_encadre("MENU", "DE CONSULTATION");
				break;
				
			case 0:
				/* retour au menu principal */
				continuer = 0;
				break;
				
			default:
				printf("\nChoix invalide.\n\n");
				break;
		}
	}
	
}




/***************************************************************************************************************/




/* menu pour l'affichage des informations sur les voies
il affiche la liste des voies et permet de choisir une voie dont consulter les détails */
void menu_voies()
{
	int choix;
	int saisie_correcte;
	int continuer = 1, continuer2 = 1;
	struct voie *a_afficher;
	struct noeud *noeud_a_afficher;

	affichage_encadre("MENU","DE CONSULTATION DES VOIES");
	
	while(continuer)
	{
		a_afficher = choisir_voie();
		
		if(a_afficher == NULL)
		{
			/* retour au menu d'affichage */
			continuer = 0;
		}
		
		else
		{
			printf("\n\nVous pouvez choisir un des noeuds suivants pour afficher ses informations.\n\n");
			while(continuer2)
			{
				noeud_a_afficher = choisir_noeud_voie(a_afficher);

				if(noeud_a_afficher == NULL)
				{
					continuer2 = 0;
				}

				else
				{
					affichage_infos_noeud(noeud_a_afficher);
				}
			}
			continuer2 = 1;
			printf("\n\n\n");
			affichage_encadre("MENU","DE CONSULTATION DES VOIES");
		}	
	}
}



/***************************************************************************************************************/





/* menu pour l'affichage des informations sur les noeuds
il affiche la liste des noeuds et permet de choisir un noeud dont consulter les détails */
void menu_noeuds()
{
	int choix;
	int saisie_correcte;
	int continuer = 1;
	struct noeud *a_afficher;

	affichage_encadre("MENU","DE CONSULTATION DES NOEUDS");
	
	while(continuer)
	{
		a_afficher = choisir_noeud();
		
		/* si l'utilisateur saisit -1, retour au menu d'affichage */
		if(a_afficher == NULL)
		{
			continuer = 0;
		}

		else
		{
			/* infos détaillées du noeud */
			affichage_infos_noeud(a_afficher);
			printf("\n\n");
			affichage_encadre("MENU","DE CONSULTATION DES NOEUDS");
		}
		
	}
}




/***************************************************************************************************************/




/* procédure affichant pour un noeud donné :
la liste des noeuds auxquels il est lié, la voie qui les relie, la distance entre eux et les règles de circulation dans le sens a_afficher => noeud_adjacent */
void affichage_infos_noeud(struct noeud *a_afficher)
{
	struct arc *p_arc;
	struct noeud *noeud_adjacent;
	
	/* en-tête de la page */
	affichage_encadre("Informations du noeud", a_afficher->nom_noeud);
	
	/* positionnement sur le premier arc partant du noeud */
	p_arc = a_afficher->debut_liste_arcs;
	
	/* si aucun arc ne part du noeud */
	if(p_arc == NULL)
	{
		printf("Le noeud %s n'est lié à aucun autre noeud.\n\n", a_afficher->nom_noeud);
	}
	
	else
	{
		/* boucle qui parcourt la liste des arcs partant du noeud a_afficher */
		while(p_arc != NULL)
		{
			/* informations générales */
			printf("\t\t---\n");
			printf("On peut se rendre de %s à %s (%d) par %s.\n", a_afficher->nom_noeud, (p_arc->p_noeud_arrivee)->nom_noeud, (p_arc->p_noeud_arrivee)->id_noeud, (p_arc->p_voie_arc)->nom_voie);
			printf("Distance : %dm\n", p_arc->longueur);
			
			/* règles de circulation */
			switch(p_arc->vehicule)
			{
				case 0:
					printf("Segment empruntable par tous types de véhicules.\n");
					break;
					
				case 1:
					printf("Segment réservé aux piétons.\n");
					break;
					
				case 2:
					printf("Segment réservé aux véhicules motorisés.\n");
					break;
			}
			
			/* passage à l'arc et au noeud adjacent suivants */
			p_arc = p_arc->p_successeur;
		}
	}
	
	printf("\n\n--------------------------------------------------------------\n");
}
