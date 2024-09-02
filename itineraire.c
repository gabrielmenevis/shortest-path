/* #######################################################
          SOUS-PROGRAMMES POUR L'ITINÉRAIRE
   #######################################################
*/




#include "declarations.h"
#include "itineraire.h"
#include "affichage.h"
#include "choix.h"



/* déclarations de procédures locales */
int calcul_itineraire(struct noeud *noeud_depart, struct noeud *noeud_arrivee, int moyen_transport);
void afficher_itineraire(struct arc *a, struct noeud *noeud_destination);
void afficher_itineraire_detaille(struct arc *a, struct noeud *noeud_destination);
void inserer_noeud(struct noeud *a_inserer, struct noeud *debut_liste);
void reinitialisation();


/* variables globales déclarées dans le fichier source principal */
extern struct noeud *premier_noeud;
extern int nb_noeuds;




/* menu permettant à l'utilisateur de choisir un noeud de départ et un noeud d'arrivée, un moyen de transport et lançant le calcul puis l'affichage de l'itinéraire */
void menu_itineraire()
{
	struct voie *voie_depart, *voie_arrivee;
	struct noeud *noeud_depart, *noeud_arrivee;
	int saisie_correcte, choix, choix_transport, choix_detail;
	int continuer;
	int itineraire_reussi;

	/* en-tête du menu */
	affichage_encadre("MENU", "D'ITINÉRAIRE");

	continuer = 1;

	/* tant que l'itinéraire n'est pas lancé et que l'utilisateur n'a pas choisi de revenir au menu principal */
	while(continuer)
	{
		noeud_depart = NULL;

		printf("Comment sélectionner le noeud de départ ?\n");
		printf("--------------------------------------------------------------\n");
		printf("\t\t1 - Sélectionner une voie dans la liste des voies\n");
		printf("\t\t2 - Chercher une voie par mot-clé\n");
		printf("\t\t0 - Retour au menu principal\n");

		/* saisie, contrôle, buffer vidé */
		printf("Votre choix : ");
		saisie_correcte = scanf("%d", &choix);
		while(getchar() != '\n');

		if(!saisie_correcte)
		{
			choix = -1;
		}

		switch(choix)
		{
			/* si choix du noeud par affichage de la liste globale des voies */
			case 1:
				/* on récupère un pointeur sur la voie choisie */
				voie_depart = choisir_voie();
				/* si la voie est bien choisie, on récupère un pointeur sur le sommet de départ */
				if(voie_depart != NULL)
				{
					noeud_depart = choisir_noeud_voie(voie_depart);
				}
				break;

			/* si choix du noeud par recherche de voies par mot-clé */
			case 2:
				/* on récupère un pointeur sur la voie choisie */
				voie_depart = choisir_voie_cherchee();
				/* si la voie est bien choisie, on récupère un pointeur sur le sommet de départ */
				if(voie_depart != NULL)
				{
					noeud_depart = choisir_noeud_voie(voie_depart);
				}
				break;

			/* si l'utilisateur souhaite quitter, on sort de la boucle */
			case 0:
				continuer = 0;
				break;

			default:
				printf("\nChoix invalide.\n\n");
				break;
		}

		/* seulement si l'utilisateur a choisi un noeud de départ, on lui demande de choisir le noeud d'arrivée */
		if(noeud_depart != NULL)
		{
			noeud_arrivee = NULL;

			printf("Comment sélectionner le noeud d'arrivée ?\n");
			printf("--------------------------------------------------------------\n");
			printf("\t\t1 - Sélectionner une voie dans la liste des voies\n");
			printf("\t\t2 - Chercher une voie par mot-clé\n");
			printf("\t\t0 - Retour au choix du noeud de départ\n");

			/* saisie, contrôle, buffer vidé */
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
					voie_arrivee = choisir_voie();
					if(voie_arrivee != NULL)
					{
						noeud_arrivee = choisir_noeud_voie(voie_arrivee);
					}
					break;

				case 2:
					voie_arrivee = choisir_voie_cherchee();
					if(voie_arrivee != NULL)
					{
						noeud_arrivee = choisir_noeud_voie(voie_arrivee);
					}
					break;

				case 0:
					/* il ne se passe rien, on reboucle simplement au début du choix des noeuds */
					break;

				default:
					printf("\nChoix invalide.\n\n");
					break;
			}

			/* si le noeud d'arrivée est bien choisi */
			if(noeud_arrivee != NULL)
			{
				/* on vérifie que les noeuds choisis sont bien distincts */
				if(noeud_arrivee == noeud_depart)
				{
					printf("\n\n/!\\ ERREUR /!\\ Veuillez choisir un point de départ distinct du point d'arrivée.\n\n");
					printf("\n\n");
					affichage_encadre("MENU", "D'ITINÉRAIRE");
				}

				/* si tout est bon, on sort de la boucle et on lance le calcul d'itinéraire */
				else
				{
					continuer = 0;
				}
			}
		}
	}


	/* si les deux noeuds sont bien choisis */
	if((noeud_depart != NULL) && (noeud_arrivee != NULL))
	{
		printf("\n\n");

		choix_transport = 0;

		/* tant qu'un moyen de transport valide n'est pas choisi */
		while((choix_transport != PIETON) && (choix_transport != VOITURE))
		{
			/* saisie, contrôle, on vide le buffer */
			printf("Voulez-vous partir à pied (1) ou en voiture (2) ? ");
			scanf("%d", &choix_transport);
			while(getchar() != '\n');

			if((choix_transport != PIETON) && (choix_transport != VOITURE))
			{
				printf("\nChoix invalide.\n\n");
			}
		}

		/* récapitulatif de l'itinéraire */
		printf("\n------------------------------------------------------\n\n");
		printf("L'itinéraire suivant sera calculé :\n");
		printf("\t\tnoeud de départ : n°%d (%s) sur %s\n", noeud_depart->id_noeud, noeud_depart->nom_noeud, voie_depart->nom_voie);
		printf("\t\tnoeud d'arrivée : n°%d (%s) sur %s\n", noeud_arrivee->id_noeud, noeud_arrivee->nom_noeud, voie_arrivee->nom_voie);
		printf("\t\tMoyen de transport : ");
		if(choix_transport == PIETON)
		{
			printf("piéton\n");
		}
		else
		{
			printf("voiture\n");
		}

		printf("\n------------------------------------------------------\n\n");

		/* calcul de l'itinéraire ; on récupère 1 si l'itinéraire existe, 0 sinon */
		itineraire_reussi = calcul_itineraire(noeud_depart, noeud_arrivee, choix_transport);

		/* s'il y a un itinéraire à afficher */
		if(itineraire_reussi)
		{
			choix_detail = 0;

			while((choix_detail != 1) && (choix_detail != 2))
			{
				printf("Itinéraire calculé avec succès. Vous avez deux options pour l'affichage :\n");
				printf("\t\t1 - Affichage simple : voie par voie\n");
				printf("\t\t2 - Affichage détaillé : arc par arc\n");

				/* saisie, contrôle, vide buffer */
				printf("Votre choix : ");
				saisie_correcte = scanf("%d", &choix_detail);
				while(getchar() != '\n');
				printf("\n\n");

				if(!saisie_correcte)
				{
					printf("Veuillez saisir un nombre.\n\n");
				}

				else
				{
					if(choix_detail == 1)
					{
						afficher_itineraire(noeud_arrivee->p_provenance, noeud_arrivee);
					}

					else if (choix_detail == 2)
					{
						afficher_itineraire_detaille(noeud_arrivee->p_provenance, noeud_arrivee);
					}

					else
					{
						printf("\nChoix invalide.\n\n");
					}
				}

			}
			
		}

		/* si le calcul de l'itinéraire n'a pas abouti */
		else
		{
		 	printf("\n\n/!\\ Il n'y a aucun itinéraire possible entre %s et %s pour le moyen de transport choisi.\n\n", noeud_depart->nom_noeud, noeud_arrivee->nom_noeud);
		}
		
	}

}





/*****************************************************************************************************/





/* fonction servant au calcul de l'itinéraire entre noeud_depart et noeud_arrivee, par moyen_transport (piéton = 1 ou voiture = 2)
elle implémente l'algorithme de Dijkstra :

- construction d'un sous-graphe partant du noeud de départ, par ajout progressif des noeuds liés aux noeuds traités

- les noeuds à traiter sont placés dans une liste chaînée par les pointeurs pred_liste_dijkstra et succ_liste_dijkstra

- la liste est triée par ordre croissant de la distance totale au départ des noeuds ajoutés au sous-graphe

- à chaque fois qu'on traite un noeud, on insère ses noeuds adjacents à la liste des noeuds à traiter ou, s'ils y sont déjà, on les déplace en fonction de leur nouvelle distance totale au départ

- un noeud n'est traité qu'une fois

- on parcourt la liste jusqu'à ce que le noeud non traité avec la distance la plus courte soit le noeud d'arrivée (on retourne 1 pour indiquer le succès du calcul)
	ou jusqu'à ce que tous les noeuds possibles aient été ajoutés au sous-graphe sans trouver le noeud d'arrivée (on retourne 0 pour indiquer l'échec du calcul) */
int calcul_itineraire(struct noeud *noeud_depart, struct noeud *noeud_arrivee, int moyen_transport)
{
	struct noeud *noeud_actuel, *noeud_adjacent;
	struct arc *arc;
	int distance_calculee;
	
	/* on se place sur le noeud de départ, pour l'instant le seul noeud du sous-graphe */
	noeud_actuel = noeud_depart;
	
	/* boucle d'itération de la liste de noeuds à parcourir, triée par distance totale la plus courte
	- si le premier élément non traité de la liste (noeud_actuel) est le noeud avec la distance la plus courte, on a trouvé le plus court chemin
	- s'il n'y a plus d'élément dans la liste (noeud_actuel == NULL), alors on a ajouté tous les noeuds possibles sans parvenir au noeud d'arrivée : pas d'itinéraire possible */
	while((noeud_actuel != noeud_arrivee) && (noeud_actuel != NULL))
	{	

		/* on se place au début de la liste des arcs du noeud actuel */
		arc = noeud_actuel->debut_liste_arcs;
		
		/* itération de la liste des arcs du noeud, pour ajouter les noeuds adjacents ou mettre à jour leur distance au départ et leur position dans la liste des noeuds à traiter */
		while(arc != NULL)
		{

			/* on n'ajoute le noeud adjacent que si le moyen de transport choisi est compatible avec la circulation sur cet arc */
			if((arc->vehicule == INDIFFERENT) || (arc->vehicule == moyen_transport))
			{

				noeud_adjacent = arc->p_noeud_arrivee;
				
				/* calcul de la distance totale du chemin vers noeud_adjacent en venant de noeud_actuel */
				distance_calculee = arc->longueur + noeud_actuel->distance_au_depart;
				
				/* si le noeud n'est pas encore ajouté au sous-graphe (distance_totale à sa valeur d'initialisation
         	ou que la distance_totale la plus courte pour l'instant est supérieure à celle qu'on calcule venant de noeud_actuel */
				if((noeud_adjacent->distance_au_depart > distance_calculee) || ((noeud_adjacent != noeud_depart) && (noeud_adjacent->distance_au_depart == 0)))
				{

					/* mise à jour de la distance au départ du noeud adjacent et de l'arc par lequel on y arrive */
					noeud_adjacent->distance_au_depart = distance_calculee;
					noeud_adjacent->p_provenance = arc;
					
					/* insertion du noeud adjacent dans la liste des prochains noeuds à traiter */
					inserer_noeud(noeud_adjacent, noeud_actuel);

				}
			}
			
			/* on passe au prochain arc partant de noeud_actuel */
			arc = arc->p_successeur;
		}
		
		/* on passe au prochain noeud à traiter dans la liste */
		noeud_actuel = noeud_actuel->succ_liste_dijkstra;
	}
	
	/* cas où le calcul d'itinéraire a réussi */
	if(noeud_actuel == noeud_arrivee)
	{
		return 1;
	}
	
	/* cas où il n'y a pas d'itinéraire possible entre les deux noeuds */
	else
	{
		return 0;
	}
	
}





/*****************************************************************************************/




/* cette procédure insère un noeud reçu en paramètre dans la liste chaînée de traitement pour le calcul de l'itinéraire */
void inserer_noeud(struct noeud *a_inserer, struct noeud *debut_liste)
{
	struct noeud *curseur;
	int continuer = 1;
	
	/* si a_inserer est déjà présent dans la liste, on le retire préalablement en chaînant ensemble son prédécesseur et son successeur */
	if(a_inserer->pred_liste_dijkstra != NULL)
	{
   	(a_inserer->pred_liste_dijkstra)->succ_liste_dijkstra = a_inserer->succ_liste_dijkstra;
	}

	if(a_inserer->succ_liste_dijkstra != NULL)
	{
   	(a_inserer->succ_liste_dijkstra)->pred_liste_dijkstra = a_inserer->pred_liste_dijkstra;
	}
  
  
	/* on se place au début de la liste des noeuds à traiter */
	curseur = debut_liste;
  
	/* on parcourt la liste dans l'ordre tant que a_inserer n'est pas inséré */
	while(continuer)
	{
  		/* si la distance au départ du noeud à insérer est inférieure à celle du curseur, on place le noeud dans la liste avant le curseur */
  		if(a_inserer->distance_au_depart < curseur->distance_au_depart)
  		{
  			a_inserer->succ_liste_dijkstra = curseur;
  			a_inserer->pred_liste_dijkstra = curseur->pred_liste_dijkstra;
  		
  			if(curseur != debut_liste)
  			{
  				(curseur->pred_liste_dijkstra)->succ_liste_dijkstra = a_inserer;
			}
			
			curseur->pred_liste_dijkstra = a_inserer;
			
			continuer = 0;
		}
		
		/* sinon si la fin de la liste à traiter est atteinte, on insère le noeud à la fin de la liste */
		else if(curseur->succ_liste_dijkstra == NULL)
		{
			curseur->succ_liste_dijkstra = a_inserer;
			a_inserer->pred_liste_dijkstra = curseur;
			
			continuer = 0;
		}
		
		/* sinon, on passe au noeud suivant dans la liste */
		else
		{
			curseur = curseur->succ_liste_dijkstra;
		}
		
	}
	
}




/*****************************************************************************************/






/* procédure récursive d'affichage de l'itinéraire calculé
elle doit être appelée à partir du dernier arc emprunté par l'itinéraire

- jusqu'à l'appel récursif, on parcourt l'itinéraire "à l'envers" jusqu'au noeud de départ en marquant les derniers arcs empruntés sur chaque voie

- on appelle afficher_itineraire sur l'arc précédent de l'itinéraire, si on n'a pas déjà atteint le départ

- après l'appel, on parcourt l'itinéraire "à l'endroit" en calculant la distance totale parcourue sur une voie et en affichant l'itinéraire voie après voie */
void afficher_itineraire(struct arc *a, struct noeud *noeud_destination)
{
	struct noeud *n1, *n2;
	static struct voie *v = NULL; // variable statique qui garde en mémoire la voie de l'arc traité précédemment (le suivant dans l'ordre de parcours départ => arrivée)
	static int longueur_voie = 0; // variable statique qui additionne la longueur des arcs successifs d'une même voie
	static struct noeud *n_depart_voie = NULL; // variable statique qui conserve le noeud de départ du premier arc parcouru pour la voie_courante
	int fin_voie; // booléen qui indique si c'est le dernier arc pour la voie courante
	
	
	/* si la procédure reçoit NULL en paramètre, elle réinitialise simplement ses variables statiques */
	if(a == NULL)
	{
		// v = NULL;
		// longueur_voie = 0;
		// n_depart_voie = NULL;
	}
	
	
	else
	{
		/* on récupère les noeuds de départ et d'arrivée de l'arc traité */
		n1 = a->p_noeud_depart;
		n2 = a->p_noeud_arrivee;
	  
		/* si la voie de l'arc ne correspond pas à celle de l'arc traité précédemment, fin_voie passe à VRAI et on note la nouvelle valeur de la voie */
		if(a->p_voie_arc != v)
		{
			fin_voie = 1;
			v = a->p_voie_arc;
		}
		
		/* sinon fin_voie est FAUX */
		else
		{
			fin_voie = 0;
		}
		
		
		
		/* appel récursif de afficher_itineraire() sur l'arc de provenance, tant qu'on n'a pas atteint le départ du parcours */
		if(n1->p_provenance != NULL)
		{
			afficher_itineraire(n1->p_provenance, noeud_destination);
		}
	  
		/* si on est au départ du parcours, le départ de la voie courante est n1 */
		else
		{
			n_depart_voie = n1;
		}
		
		
		
		/* si la voie de l'arc n'est pas la même que celui traité précédemment, c'est le premier arc d'une nouvelle voie
		on conserve n1 comme le départ de cette voie, on réinitialise la longueur à 0, et on note la nouvelle valeur de la voie */
		if(a->p_voie_arc != v)
		{
	  		n_depart_voie = n1;
	  		longueur_voie = 0;
	  		v = a->p_voie_arc;
		}
		
		/* on ajoute à la longueur de la voie celle de l'arc traité */
		longueur_voie += a->longueur;
	
		/* affichage du segment d'itinéraire si l'arc n'appartient pas à la même voie que l'arc suivant */
		if(fin_voie)
		{
	   		printf("Prenez %s de %s à %s ", v->nom_voie, n_depart_voie->nom_noeud, n2->nom_noeud);

	    	/* formatage de l'affichage de la distance : en mètres si inférieure à 1km, en km avec deux décimales sinon (arrondi supérieur) */
			if(longueur_voie < 1000)
			{
	    		printf("pendant %dm.\n", longueur_voie);
	   		}

			else
			{
	    		printf("pendant %.2fkm.\n", (longueur_voie/1000.0));
	   		}
		}

		/* l'itinéraire est fini */
		if(n2 == noeud_destination)
		{
			printf("\n\nDistance totale parcourue : %d mètres.\n\n", n2->distance_au_depart);
			reinitialisation(); // réinitialiser les variables pour le calcul de l'itinéraire
			v = NULL; // réinitialiser les variables statiques pour l'affichage de l'itinéraire
			longueur_voie = 0;
			n_depart_voie = NULL;
		}
	}
}





/*****************************************************************************************/






/* cette fonction affiche l'itinéraire arc par arc
c'est une fonction récursive qui suit la même logique que afficher_itineraire() mais n'effectue pas de tests sur la voie
car elle ne cherche pas à regrouper les arcs situés sur une même voie dans un seul affichage */
void afficher_itineraire_detaille(struct arc *a, struct noeud *noeud_destination)
{
	struct noeud *n1, *n2;
	struct voie *v;

	/* on récupère les noeuds de départ et d'arrivée de l'arc traité */
	n1 = a->p_noeud_depart;
	n2 = a->p_noeud_arrivee;

	/* on récupère la voie de l'arc traité */
	v = a->p_voie_arc;

	/* appel récursif de afficher_itineraire_detaille() sur l'arc de provenance, tant qu'on n'a pas atteint le départ du parcours */
	if(n1->p_provenance != NULL)
	{
		afficher_itineraire_detaille(n1->p_provenance, noeud_destination);
	}

	/* affichage des informations de l'arc */
	printf("Prenez %s de %s à %s pendant %dm.\n", v->nom_voie, n1->nom_noeud, n2->nom_noeud, a->longueur);

	/* l'itinéraire est fini */
	if(n2 == noeud_destination)
	{
		printf("\n\nDistance totale parcourue : %d mètres.\n\n", n2->distance_au_depart);
		reinitialisation(); // réinitialiser les variables pour le calcul de l'itinéraire
	}
}




/*****************************************************************************************/





/* cette fonction (ré)initialise les données nécessaires au calcul de l'itinéraire :
- elle parcourt la liste globale des noeuds et remet à 0 ou NULL leurs attributs spécifiques au calcul de l'itinéraire */
void reinitialisation()
{
	struct noeud *a_reinitialiser;
	
	/* test sur l'existence de noeuds à réinitialiser */
	if(nb_noeuds == 0)
	{
		printf("Aucun noeud à réinitialiser\n");
	}
	
	/* parcours complet de la liste chaînée des noeuds et réinitialisation de leurs attributs */
	else
	{
		a_reinitialiser = premier_noeud;
		
		while(a_reinitialiser != NULL)
		{
			a_reinitialiser->distance_au_depart = 0;
			a_reinitialiser->p_provenance = NULL;
			a_reinitialiser->pred_liste_dijkstra = NULL;
			a_reinitialiser->succ_liste_dijkstra = NULL;

			a_reinitialiser = a_reinitialiser->succ_liste_globale;
		}
	}
}
