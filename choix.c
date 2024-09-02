/* #######################################################
          CHOIX DE VOIES ET NOEUDS
   #######################################################
*/



#include "declarations.h"
#include "choix.h"
#include "acces.h"
#include "affichage.h"
#include "listes.h"



/* déclaration des procédures locales */
int verif_noeud_sur_voie(struct noeud *p_noeud, struct voie *p_voie);
void conv_maj(char chaine[]);

/* variables globales */
extern struct voie *premiere_voie;
extern struct noeud *premier_noeud;
extern int nb_voies;


/* cette fonction affiche la liste entière des voies, propose à l'utilisateur d'en choisir une et renvoie un pointeur sur la voie choisie
elle renvoie NULL si l'utilisateur renonce à choisir une voie */
struct voie *choisir_voie()
{
	struct voie *voie_choisie;
	int choix_voie;
	int continuer;
	int saisie_correcte;

	/* liste des voies */
	liste_voies();

	continuer = 1;

	/* tant que l'utilisateur n'a pas choisi de voie ni choisi de quitter */
	while(continuer)
	{
		/* saisie, contrôle du format, buffer vidé */
		printf("\nQuelle voie de la liste choisissez-vous (-1 pour revenir en arrière) ? ");
		saisie_correcte = scanf("%d", &choix_voie);
		while(getchar() != '\n');

		if(!saisie_correcte)
		{
			printf("\nVeuillez saisir un nombre.\n\n");
		}

		/* si l'utilisateur quitte, on sort de la boucle et voie_choisie prend NULL */
		else if(choix_voie == -1)
		{
			voie_choisie = NULL;
			continuer = 0;
		}

		else
		{
			/* on récupère le pointeur vers la voie dont l'utilisateur a choisi l'id */
			voie_choisie = acces_voie(choix_voie);

			/* message d'erreur si l'id ne correspond à aucune voie */
			if(voie_choisie == NULL)
			{
				printf("\nCette voie n'existe pas. Choisir un numéro parmi les voies indiquées.\n\n");
			}

			/* sinon la voie est choisie : on sort de la boucle */
			else
			{
				continuer = 0;
			}
		}
	}

	return voie_choisie;
}




/****************************************************************************************/





/* cette fonction demande à l'utilisateur de saisir un mot-clé, cherche les voies dont le nom contient ce mot-clé et les affiche
puis, elle demande à l'utilisateur de choisir une voie parmi les voies affichées, contrôle la saisie
elle retourne un pointeur sur la voie choisie */
struct voie *choisir_voie_cherchee()
{
	struct voie *voie_courante, *voie_choisie;
	char mot_cle[MAX_NOM_VOIE], mot_cle_maj[MAX_NOM_VOIE], nom_voie_maj[MAX_NOM_VOIE];
	char *voie_trouvee;
	int continuer, continuer2;
	int nb_voies_trouvees;
	int choix, choix_voie;
	int saisie_correcte;

	/* initialisation à NULL du pointeur voie à retourner */
	voie_choisie = NULL;

	/* s'il n'y a pas de voies dans lesquelles lancer une recherche */
	if(nb_voies == 0)
	{
		printf("Aucune voie chargée.\n\n");
	}

	else
	{
		continuer = 1;

		/* boucle de choix de mot-clé */
		while(continuer)
		{
			/* (ré)initialisation du nombre de voies trouvées avant chaque nouvelle recherche */
			nb_voies_trouvees = 0;

			/* saisie du mot-clé et vidage du buffer */
			printf("Saisissez un mot-clé à chercher dans les voies : ");
			scanf("%[^\n]s", mot_cle);
			while(getchar() != '\n');

			/* conversion du mot-clé en majuscules pour éviter les problèmes de casse */
			strcpy(mot_cle_maj, mot_cle);
			conv_maj(mot_cle_maj);
			
			/* positionnement au début de la liste des voies */
			voie_courante = premiere_voie;

			/* parcours de la liste des voies */
			while(voie_courante != NULL)
			{
				/* conversion du nom de la voie en majuscules pour éviter les problèmes de casse */
				strcpy(nom_voie_maj, voie_courante->nom_voie);
				conv_maj(nom_voie_maj);

				/* on vérifie si le nom de la voie contient le mot-clé saisi */
				voie_trouvee = strstr(nom_voie_maj, mot_cle_maj);

				/* si le nom contient le mot clé, on affiche la voie */
				if(voie_trouvee != NULL)
				{
					/* incrémentation du nombre de voies trouvées */
					nb_voies_trouvees++;

					/* si c'est la première voie affichée, on affiche l'en-tête de la liste */
					if(nb_voies_trouvees == 1)
					{
						printf("---------------------------------------------------------------------\n");
						printf("%-20s|%17s\n", "NUMÉRO DE LA VOIE", "NOM DE LA VOIE");
						printf("---------------------------------------------------------------------\n");
					}

					/* affichage des informations de la voie */
					printf("%17d   |   %s\n", voie_courante->id_voie, voie_courante->nom_voie);
				}

				voie_courante = voie_courante->succ_liste_globale;

			}

			/* initialisation du choix en vue des boucles */
			choix = 5;

			/* si aucune voie n'a été trouvée, on propose de revenir en arrière (retour au menu du choix de noeud) ou de chercher un nouveau mot-clé */
			if(nb_voies_trouvees == 0)
			{
				printf("\n\nAucune voie ne contient le mot-clé %s.\n", mot_cle);

				while((choix != -1) && (choix != 1))
				{
					printf("Souhaitez-vous lancer une nouvelle recherche (1) ou revenir en arrière (-1) ? ");
					scanf("%d", &choix);
					while(getchar() != '\n');

					if((choix != -1) && (choix != 1))
					{
						printf("\nChoix invalide.\n\n");
					}
				}
			}

			/* sinon on propose en plus de choisir une voie parmi la liste affichée */
			else
			{
				printf("---------------------------------------------------------------------\n\n");

				printf("%d voies trouvées pour le mot-clé %s.\n\n", nb_voies_trouvees, mot_cle);

				while((choix != -1) && (choix != 1) && (choix != 2))
				{
					printf("Souhaitez-vous lancer une nouvelle recherche (1), choisir une voie parmi les voies affichées (2) ou revenir en arrière (-1) ? ");
					scanf("%d", &choix);
					while(getchar() != '\n');

					if((choix != -1) && (choix != 1) && (choix != 2))
					{
						printf("\nChoix invalide.\n\n");
					}
				}
			}

			/* si l'utilisateur veut revenir en arrière on sort de la boucle et on quitte la fonction qui retournera NULL */
			if(choix == -1)
			{
				continuer = 0;
			}

			/* si l'utilisateur veut lancer une nouvelle recherche on saute deux lignes */
			else if(choix == 1)
			{
				printf("\n\n");
			}

			/* si l'utilisateur veut choisir une voie parmi la liste affichée */
			else
			{
				continuer2 = 1;

				/* boucle pour choisir une voie */
				while(continuer2)
				{
					printf("\n\nChoisir un numéro de voie parmi les voies affichées (-1 pour revenir en arrière) : ");
					saisie_correcte = scanf("%d", &choix_voie);
					while(getchar() != '\n');

					if(!saisie_correcte)
					{
						printf("\nVeuillez saisir un nombre.\n\n");
					}

					else
					{
						/* si l'utilisateur veut revenir en arrière (saisie d'un nouveau mot-clé), on quitte la boucle de choix de la voie */
						if(choix_voie == -1)
						{
							continuer2 = 0;
						}

						else
						{
							/* on récupère un pointeur sur la voie correspondant à l'id choisi */
							voie_choisie = acces_voie(choix_voie);

							/* si l'id ne correspond à aucune voie chargée */
							if(voie_choisie == NULL)
							{
								printf("\nLa voie %d n'existe pas.\n", choix_voie);
							}

							else
							{
								/* on vérifie que le nom de la voie choisie contient bien le mot-clé cherché pour s'assurer que la voie fait partie de la liste affichée */
								strcpy(nom_voie_maj, voie_choisie->nom_voie);
								conv_maj(nom_voie_maj);
								voie_trouvee = strstr(nom_voie_maj, mot_cle_maj);

								if(voie_trouvee == NULL)
								{
									printf("\nLa voie %d | %s ne figure pas dans la liste.\n", voie_choisie->id_voie, voie_choisie->nom_voie);
								}

								/* si la voie choisie fait bien partie de la liste affichée, on sort des deux boucles (choix de voie et choix de mot-clé)
								on retourne un pointeur non NULL sur voie_choisie */
								else
								{
									continuer = 0;
									continuer2 = 0;
								}
							}
						}
					}
				}
			}
		}
	}

	return voie_choisie;

}





/**************************************************************************************/





/* cette fonction affiche la liste des noeuds situés sur une voie
elle demande à l'utilisateur de choisir un de ces noeuds, contrôle la saisie et renvoie un pointeur vers le noeud choisi
elle renvoie NULL si l'utilisateur décide de quitter le choix de noeud */
struct noeud *choisir_noeud_voie(struct voie *p_voie)
{
	struct noeud *p_noeud, *noeud_choisi;
	int noeud_sur_voie;
	int choix_noeud;
	int continuer;
	int saisie_correcte;
	int nb_noeuds_voie;

	nb_noeuds_voie = 0;

	affichage_encadre("Liste des noeuds sur",p_voie->nom_voie);

	/* on parcourt la liste globale des noeuds */
	p_noeud = premier_noeud;

	while(p_noeud != NULL)
	{
		/* on récupère 1 si le noeud est sur p_voie, 0 sinon */
		noeud_sur_voie = verif_noeud_sur_voie(p_noeud, p_voie);

		/* si le noeud est sur la voie, on l'affiche */
		if(noeud_sur_voie)
		{
			printf("\t\t- noeud %d : %s\n", p_noeud->id_noeud, p_noeud->nom_noeud);
			nb_noeuds_voie++;
		}

		/* on passe au noeud suivant */
		p_noeud = p_noeud->succ_liste_globale;
	}

	printf("\n\n");

	continuer = 1;

	/* message et fin de la fonction si aucun noeud n'est sur la voie ; on retourne NULL*/
	if(nb_noeuds_voie == 0)
	{
		printf("Aucun noeud situé sur la voie %d | %s\n", p_voie->id_voie, p_voie->nom_voie);
		printf("Retour au choix de la voie.\n\n");
		noeud_choisi = NULL;
	}

	else
	{
		/* tant que l'utilisateur n'a pas choisi de noeud ou choisi de revenir en arrière */
		while(continuer)
		{
			/* saisie, contrôle de la saisie, buffer vidé */
			printf("Quel noeud choisir (-1 pour revenir en arrière) ? ");
			saisie_correcte = scanf("%d", &choix_noeud);
			while(getchar() != '\n');

			if(!saisie_correcte)
			{
				printf("\nVeuillez saisir un nombre.\n\n");
			}

			/* l'utilisateur souhaite quitter : on sort de la boucle et on retourne NULL */
			else if(choix_noeud == -1)
			{
				noeud_choisi = NULL;
				continuer = 0;
			}

			else
			{
				/* on récupère un pointeur sur le noeud dont l'id a été saisi */
				noeud_choisi = acces_noeud(choix_noeud);

				/* si l'id ne correspond à aucun noeud */
				if(noeud_choisi == NULL)
				{
					printf("\nLe noeud n°%d n'existe pas.\n\n", choix_noeud);
				}

				else
				{
					/* on vérifie que le noeud choisi est bien sur la voie demandée */
					noeud_sur_voie = verif_noeud_sur_voie(noeud_choisi, p_voie);

					/* sinon, on demande à saisir un nouveau noeud */
					if(!noeud_sur_voie)
					{
						printf("\nLe noeud %d | %s n'appartient pas à la voie %d | %s\n\n", noeud_choisi->id_noeud, noeud_choisi->nom_noeud, p_voie->id_voie, p_voie->nom_voie);
					}

					/* si le noeud est bien positionné sur la voie, on sort de la boucle et on retourne un pointeur sur le noeud */
					else
					{
						continuer = 0;
					}
				}
			}
		}
	}

	return noeud_choisi;
}




/************************************************************************************************/





/* fonction qui vérifie qu'un noeud est sur une voie (pointeurs reçus en paramètre)
retourne 1 si c'est le cas, sinon 0 */
int verif_noeud_sur_voie(struct noeud *p_noeud, struct voie *p_voie)
{
	struct arc *p_arc;
	int noeud_sur_voie;

	noeud_sur_voie = 0;

	/* on parcourt la liste des arcs du noeud jusqu'à avoir atteint le dernier arc ou jusqu'à ce qu'on trouve un arc situé sur la voie demandée */
	p_arc = p_noeud->debut_liste_arcs;

	while((p_arc != NULL) && (noeud_sur_voie == 0))
	{
		/* si l'arc est situé sur la bonne voie, la valeur à retourner passe à 1 */
		if(p_arc->p_voie_arc == p_voie)
		{
			noeud_sur_voie = 1;
		}

		p_arc = p_arc->p_successeur;
	}

	return noeud_sur_voie;
}





/*****************************************************************************************************/





/* cette procédure convertit en majuscules une chaîne de caractères reçue en paramètre */
void conv_maj(char chaine[])
{
	int i, longueur;

	longueur = strlen(chaine);

	for(i = 0 ; i < longueur ; i++)
	{
		chaine[i] = toupper(chaine[i]);
	}
}





/**************************************************************************************/





/* affichage de la liste des noeuds et choix d'un noeud dans la liste
retourne NULL si l'utilisateur renonce à choisir un noeud */
struct noeud *choisir_noeud()
{
	struct noeud *noeud_choisi;
	int choix_noeud;
	int continuer;
	int saisie_correcte;

	/* liste des noeuds */
	liste_noeuds();

	continuer = 1;

	/* tant que l'utilisateur n'a pas choisi de noeud ni choisi de quitter */
	while(continuer)
	{
		/* saisie, contrôle du format, buffer vidé */
		printf("\nQuel noeud de la liste choisissez-vous (-1 pour revenir en arrière) ? ");
		saisie_correcte = scanf("%d", &choix_noeud);
		while(getchar() != '\n');

		if(!saisie_correcte)
		{
			printf("\nVeuillez saisir un nombre.\n\n");
		}

		/* si l'utilisateur quitte, on sort de la boucle et noeud_choisi prend NULL */
		else if(choix_noeud == -1)
		{
			noeud_choisi = NULL;
			continuer = 0;
		}

		else
		{
			/* on récupère le pointeur vers le noeud dont l'utilisateur a choisi l'id */
			noeud_choisi = acces_noeud(choix_noeud);

			/* message d'erreur si l'id ne correspond à aucun noeud */
			if(noeud_choisi == NULL)
			{
				printf("\nCe noeud n'existe pas. Choisir un numéro parmi les noeuds indiqués.\n\n");
			}

			/* sinon le noeud est choisi : on sort de la boucle */
			else
			{
				continuer = 0;
			}
		}
	}

	return noeud_choisi;
}