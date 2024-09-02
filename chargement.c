/* ################################
   SOUS-PROGRAMMES DE CHARGEMENT
   ################################
*/



#include "declarations.h"
#include "chargement.h"
#include "affichage.h"
#include "acces.h"

#define FICHIER_VOIES "voies_13.txt"
#define FICHIER_NOEUDS "noeuds_13.txt"
#define FICHIER_ARCS "arcs-13.txt"


/* déclaration des fonctions locales */
int chargement_noeuds(char nom_fichier [MAX_NOM_FICHIER]);
int chargement_voies(char nom_fichier [MAX_NOM_FICHIER]);
int chargement_arcs(char nom_fichier [MAX_NOM_FICHIER]);
int lier_arcs_noeuds(int id_noeud_provenance, int id_noeud_destination, struct arc *a_inserer);


/* variables globales déclarées dans le fichier source principal */
extern struct voie *premiere_voie, *derniere_voie;
extern struct noeud *premier_noeud, *dernier_noeud;
extern int nb_voies;
extern int nb_noeuds;
extern int nb_arcs;



/* fonction qui demande à l'utilisateur le nom des fichiers à charger et lance les trois chargements successifs
renvoie 1 si tous les chargements ont réussi, 0 si l'un des chargements a échoué */
int chargement_global()
{
	char nom_fichier_voies[MAX_NOM_FICHIER], nom_fichier_noeuds[MAX_NOM_FICHIER], nom_fichier_arcs[MAX_NOM_FICHIER];
	int choix = -1;
	int chargement_voies_reussi, chargement_noeuds_reussi, chargement_arcs_reussi, chargement_global_reussi;

	/* en-tête du menu */
	affichage_encadre("MENU","DE CHARGEMENT");
	
	/* libération préalable de la mémoire des données éventuellement déjà chargées */
	liberer_memoire();
	
	/* information sur le nom des fichiers par défaut */
	printf("Les données seront chargées par défaut dans les fichiers :\n");
	printf("\t- %s pour les voies\n", FICHIER_VOIES);
	printf("\t- %s pour les noeuds\n", FICHIER_NOEUDS);
	printf("\t- %s pour les arcs\n\n", FICHIER_ARCS);
	
	/* proposition pour la saisie de noms personnalisés */
	while((choix != 0) && (choix != 1))
	{
		printf("Souhaitez-vous charger un autre jeu de données (0 NON / 1 OUI) ? ");
		scanf("%d", &choix);
		while(getchar() != '\n');
		if((choix != 0) && (choix != 1))
		{
			printf("Choix invalide\n\n");
		}
		
	}
	
	/* saisie de noms personnalisés pour les fichiers de données */
	if(choix == 1)
	{
		printf("\nEntrez maintenant les noms des fichiers que vous souhaitez charger.\n");

		printf("Fichier des voies : ");
		scanf("%s", nom_fichier_voies);
		while(getchar() != '\n');
		printf("Fichier des noeuds : ");
		scanf("%s", nom_fichier_noeuds);
		while(getchar() != '\n');
		printf("Fichier des arcs : ");
		scanf("%s", nom_fichier_arcs);
		while(getchar() != '\n');
	}
	
	/* valeurs par défaut des noms de fichiers si l'utilisateur ne souhaite pas charger d'autres fichiers */
	else
	{
		strcpy(nom_fichier_voies, "voies_13.txt");
		strcpy(nom_fichier_noeuds, "noeuds_13.txt");
		strcpy(nom_fichier_arcs, "arcs_13.txt");
	}

	/* initialisation à 0 de la réussite du chargement global */
	chargement_global_reussi = 0;
	
	/* chargement des voies */
	chargement_voies_reussi = chargement_voies(nom_fichier_voies);

	/* chargement des noeuds si le chargement des voies est réussi */
	if(chargement_voies_reussi)
	{
		chargement_noeuds_reussi = chargement_noeuds(nom_fichier_noeuds);

		/* chargement des arcs si le chargement des noeuds est réussi */
		if(chargement_noeuds_reussi)
		{
			chargement_arcs_reussi = chargement_arcs(nom_fichier_arcs);

			/* chargement_global_reussi prend 1 si tous les chargements ont été réussis */
			if(chargement_arcs_reussi)
			{
				chargement_global_reussi = 1;
			}
		}
	}

	/* message d'erreur global avertissant que le programme va s'arrêter */
	if(!chargement_global_reussi)
	{
		printf("\n\n");
		printf("/!\\ ERREUR /!\\\n");
		printf("Le programme a rencontré une erreur dans le chargement des données.\n");
		printf("Nous vous invitons a verifier le bon format de vos fichiers avant de relancer le programme.\n\n");
	}
	
	/* on retourne 1 si tout a bien marché, 0 si une erreur a été détectée dans l'un des processus */
	return chargement_global_reussi;
}



/***************************************************************************************************************/



/* fonction qui charge les voies dans le fichier dont le nom est reçu en paramètre
la fonction crée des pointeurs de type struct voie et les range dans une liste chaînée
elle retourne 1 si le chargement est réussi et 0 s'il y a eu un problème durant le chargement */
int chargement_voies(char nom_fichier[MAX_NOM_FICHIER])
{
	int chargement_reussi = 1;
	FILE *f_voies;
	struct voie *nouvelle_voie, *voie_temp;
	int cpt_voies;
	int id;
	char nom[MAX_NOM_VOIE];
	
	/* ouverture en lecture seule du fichier des voies */
	f_voies = fopen(nom_fichier, "r");
	
	/* message d'erreur si pb d'ouverture, initialisation à 0 de la variable à retourner */
	if(f_voies == NULL)
	{
		printf("Erreur à l'ouverture du fichier des voies %s\n\n", nom_fichier);
		chargement_reussi = 0;
	}
	
	else
	{
		/* initialisation du compteur à 0 */
		cpt_voies = 0;
		
		voie_temp = NULL;
			
		/* boucle de lecture du fichier */
		while(!feof(f_voies) && chargement_reussi)
		{
			/* lecture et rangement des informations de la nouvelle voie */
			chargement_reussi = fscanf(f_voies, "%d,%[^\n]s", &id, nom);

			/* si la fin du fichier n'est pas atteinte */
			if(chargement_reussi != EOF)
			{
				/* allocation d'un nouvel espace mémoire pour la voie lue et mémorisation de son adresse dans nouvelle_voie */
				nouvelle_voie = (struct voie *) malloc(sizeof(struct voie));

				/* affectation des valeurs lues à la nouvelle voie */
				nouvelle_voie->id_voie = id;
				strcpy(nouvelle_voie->nom_voie, nom);

				/* on vérifie que les id_voie sont bien une suite d'entiers consécutifs */
				if(nouvelle_voie->id_voie != cpt_voies)
				{
					printf("Mauvais identifiant à la ligne %d du fichier des voies.\n", (cpt_voies+1));
					printf("Les identifiants de voies doivent être uniques, consécutifs et partir de 0\n");
					chargement_reussi = 0;
				}
				
				else
				{
					/* si p_voie_temp = NULL, alors nouvelle_voie est la premiere_voie de la liste */
					if(voie_temp == NULL)
					{
						nouvelle_voie->pred_liste_globale = NULL;
						premiere_voie = nouvelle_voie;
					}
					
					/* sinon on chaîne nouvelle_voie à la voie_temp précédemment créée */
					else
					{
						nouvelle_voie->pred_liste_globale = voie_temp;
						voie_temp->succ_liste_globale = nouvelle_voie;
					}
					
					/* en vue de la prochaine itération on conserve l'adresse de nouvelle_voie dans voie_temp */
					voie_temp = nouvelle_voie;
				}

				/* incrémentation du compteur des voies */
				cpt_voies++;
				
			}
			
		}
		
		/* fermeture du fichier */
		fclose(f_voies);
		
		/* à la fin de la lecture, on note les informations de la derniere_voie */
		nouvelle_voie->succ_liste_globale = NULL;
		derniere_voie = nouvelle_voie;
		
		/* message d'erreur si une ligne a été mal lue */
		if(!chargement_reussi)
		{
			printf("Erreur de format dans le fichier des voies. Merci de vérifier le fichier.\n");
		}
		
		/* si tout s'est bien passé, on passe à 1 la valeur de la variable à retourner et on conserve le nombre de voies chargées */
		else
		{
			printf("%d voies chargées avec succès dans %s.\n", cpt_voies, nom_fichier);
			nb_voies = cpt_voies;
			chargement_reussi = 1;
		}	
		
	}
	
	return chargement_reussi;
}



/***************************************************************************************************************/



/* fonction qui charge les noeuds dans le fichier dédié et les insère dans la liste chaînée globale des noeuds chargés
elle renvoie 1 si le chargement s'est correctement déroulé, sinon 0 */
int chargement_noeuds(char nom_fichier[MAX_NOM_FICHIER])
{
	int chargement_reussi = 1;
	FILE *f_noeuds;
	int cpt_noeuds;
	int id;
	char nom[MAX_NOM_NOEUD];
	struct noeud *nouveau_noeud, *noeud_temp;
	
	/* ouverture en lecture seule du fichier des noeuds */
	f_noeuds = fopen(nom_fichier, "r");
	
	/* message d'erreur si le fichier n'est pas ouvert */
	if(f_noeuds == NULL)
	{
		printf("Erreur à l'ouverture du fichier des noeuds %s\n\n", nom_fichier);
		chargement_reussi = 0;
	}
	
	else
	{
		/* initialisation du compteur à zéro */
		cpt_noeuds = 0;
		
		noeud_temp = NULL;
		
		/* boucle de lecture du fichier */
		while(!feof(f_noeuds) && chargement_reussi)
		{
			/* lecture des informations du nouveau noeud */
			chargement_reussi = fscanf(f_noeuds, "%d,%[^\n]s", &id, nom);

			/* si on n'a pas atteint la fin du fichier */
			if(chargement_reussi != EOF)
			{
				/* allocation d'un nouvel espace mémoire pour le noeud lu et mémorisation de son adresse dans nouveau_noeud */
				nouveau_noeud = (struct noeud *) malloc(sizeof(struct noeud));

				/* affectation des valeurs lues au noeud créé */
				nouveau_noeud->id_noeud = id;
				strcpy(nouveau_noeud->nom_noeud, nom);

				/* on vérifie que les id_noeud sont bien une suite d'entiers consécutifs */
				if(nouveau_noeud->id_noeud != cpt_noeuds)
				{
					printf("Mauvais identifiant à la ligne %d du fichier des noeuds.\n", (cpt_noeuds+1));
					printf("Les identifiants de noeuds doivent être uniques, consécutifs et partir de 0\n");
					chargement_reussi = 0;
				}
				
				else
				{
					/* si noeud_temp = NULL, alors nouveau_noeud est le premier_noeud de la liste */
					if(noeud_temp == NULL)
					{
						nouveau_noeud->pred_liste_globale = NULL;
						premier_noeud = nouveau_noeud;
					}
					
					/* sinon on chaîne nouveau_noeud au noeud_temp précédemment créé */
					else
					{
						nouveau_noeud->pred_liste_globale = noeud_temp;
						noeud_temp->succ_liste_globale = nouveau_noeud;
					}
					
					/* initialisation des attributs du noeud qui seront utilisés lors du calcul de l'itinéraire */
					nouveau_noeud->distance_au_depart = 0;
					nouveau_noeud->debut_liste_arcs = NULL;
					nouveau_noeud->p_provenance = NULL;
					nouveau_noeud->pred_liste_dijkstra = NULL;
					nouveau_noeud->succ_liste_dijkstra = NULL;
					
					/* en vue de la prochaine itération on conserve l'adresse de nouveau_noeud dans noeud_temp */
					noeud_temp = nouveau_noeud;
				}

				/* incrémentation du compteur des noeuds */
				cpt_noeuds++;
				
			}
			
		}
		
		/* fermeture du fichier */
		fclose(f_noeuds);
		
		/* à la fin de la lecture, on note les informations du dernier_noeud */
		nouveau_noeud->succ_liste_globale = NULL;
		dernier_noeud = nouveau_noeud;
		
		/* message d'erreur si une ligne a été mal lue */
		if(!chargement_reussi)
		{
			printf("Erreur de format dans le fichier des noeuds. Merci de vérifier le fichier.\n");
		}
		
		/* si tout s'est bien passé, on passe à 1 la valeur de la variable à retourner et on conserve le nombre de noeuds chargés */
		else
		{
			printf("%d noeuds chargés avec succès dans %s.\n", cpt_noeuds, nom_fichier);
			nb_noeuds = cpt_noeuds;
			chargement_reussi = 1;
		}	
		
	}
	
	
	return chargement_reussi;
}



/***************************************************************************************************************/



/* fonction qui charge le fichier des arcs ; pour chaque ligne lue dans le fichier des arcs, elle :

- crée un espace mémoire pour un nouvel arc
- appelle une fonction qui fait pointer l'arc sur la voie et les noeuds correspondants et l'insère dans la liste des arcs du noeud de départ
- réitère les opérations précédentes pour l'arc correspondant au sens inverse, en adaptant les règles de circulation selon les informations lues

la fonction renvoie 1 si le chargement s'est correctement déroulé, sinon 0 */
int chargement_arcs(char nom_fichier[MAX_NOM_FICHIER])
{
	int chargement_reussi = 1;
	FILE *f_arcs;
	int cpt_arcs;
	int id_s1, id_s2, id_voie, longueur, vehicule, sens_unique;
	struct arc *nouvel_arc, *arc_inverse;
	
	/* ouverture du fichier des arcs */
	f_arcs = fopen(nom_fichier, "r");
	
	/* affichage d'un message d'erreur si le fichier n'est pas accessible */
	if(f_arcs == NULL)
	{
		printf("Erreur à l'ouverture du fichier des arcs %s\n", nom_fichier);
		chargement_reussi = 0;
	}
	
	else
	{
		cpt_arcs = 0;
		
		/* lecture du fichier */
		while(!feof(f_arcs) && chargement_reussi)
		{	
			/* chargement des informations de la ligne lue et vérification de la bonne lecture */
			chargement_reussi = fscanf(f_arcs, "%d,%d,%d,%d,%d,%d", &id_s1, &id_s2, &id_voie, &longueur, &sens_unique, &vehicule);

			/* si on n'a pas atteint la fin du fichier */
			if(chargement_reussi != EOF)
			{
				/* message d'erreur si la lecture échoue */
				if(!chargement_reussi)
				{
					printf("\n");
					printf("Problème de lecture à la ligne %d du fichier des arcs %s. Merci de vérifier le format.\n", (cpt_arcs/2), nom_fichier);
				}
				
				/* message d'erreur si la longueur lue est négative ou nulle */
				if(longueur < 1)
				{
					printf("\n");
					printf("Problème à la ligne %d du fichier des arcs.\n", ((cpt_arcs/2)+1));
					printf("Un segment de rue entre deux croisements ne peut avoir une longueur nulle ou négative. Merci de vérifier le fichier des arcs %s.\n", nom_fichier);
					chargement_reussi = 0;
				}
				
				/* message d'erreur si sens_unique a une valeur incorrecte */
				if((sens_unique != 0) && (sens_unique != 1))
				{
					printf("\n");
					printf("Problème à la ligne %d du fichier des arcs.\n", ((cpt_arcs/2)+1));
					printf("La donnée relative au sens de circulation sur un arc (avant-dernière colonne d'une ligne) ne prend que deux valeurs : ");
					printf("1 si la circulation est différente selon le sens, 0 si elle est identique. ");
					printf("Merci de vérifier le fichier des arcs %s.\n", nom_fichier);
					chargement_reussi = 0;
				}
				
				/* message d'erreur si vehicule a une valeur incorrecte */
				if((vehicule != INDIFFERENT) && (vehicule != PIETON) && (vehicule != VOITURE))
				{
					printf("\n");
					printf("Problème à la ligne %d du fichier des arcs.\n", ((cpt_arcs/2)+1));
					printf("La donnée relative aux règles de circulation sur un arc (dernière colonne d'une ligne) ne prend que trois valeurs : ");
					printf("0 si tout le monde peut y circuler, 1 si seuls les piétons peuvent y circuler, 2 si seules les voitures peuvent y circuler. ");
					printf("Merci de vérifier le fichier des arcs %s.\n", nom_fichier);
					chargement_reussi = 0;
				}
				
				/* si aucune erreur n'est apparue, on poursuit la création de l'arc */
				if(chargement_reussi)
				{
					/* allocation d'un nouvel espace mémoire à chaque ligne lue */
					nouvel_arc = (struct arc *) malloc(sizeof(struct arc));

					nouvel_arc->longueur = longueur;
					nouvel_arc->vehicule = vehicule;

					/* on assigne à l'arc la voie correspondant à l'id lu */
					nouvel_arc->p_voie_arc = acces_voie(id_voie);
					
					/* message d'erreur si l'id ne correspond à aucune voie chargée */
					if(nouvel_arc->p_voie_arc == NULL)
					{
						printf("Problème lors du chargement des arcs à la ligne %d. Voie %d non trouvée\n", ((cpt_arcs/2) + 1), id_voie);
						chargement_reussi = 0;
					}
					
					else
					{
						/* récupération des noeuds de départ et d'arrivée et insertion de l'arc dans la chaîne du noeud de départ */
						chargement_reussi = lier_arcs_noeuds(id_s1, id_s2, nouvel_arc);
						
						/* si l'opération a réussi, on crée un duplicata de l'arc dans l'autre sens */
						if(chargement_reussi)
						{
							/* allocation d'un nouvel espace mémoire */
							arc_inverse = (struct arc *) malloc(sizeof(struct arc));
							
							/* on insère l'arc inversé dans la liste des arcs du noeud 2 qui est son noeud de départ */
							chargement_reussi = lier_arcs_noeuds(id_s2, id_s1, arc_inverse);

							/* si l'opération a réussi, on continue la création de l'arc inversé */
							if(chargement_reussi)
							{
								/* l'arc inversé se rapporte à la même voie et a la même longueur que l'arc présent en fichier */
								arc_inverse->p_voie_arc = nouvel_arc->p_voie_arc;
								arc_inverse->longueur = nouvel_arc->longueur;
								
								/* si l'arc n'est pas à sens unique, les règles de circulation sont les mêmes dans les deux sens
								NB : ce cas englobe tous les arcs dont vehicule vaut 2 (VOITURE) car il n'existe pas de segment de voie
								que les piétons puissent emprunter dans un sens mais pas dans l'autre */
								if(!sens_unique)
								{
									arc_inverse->vehicule = nouvel_arc->vehicule;
								}
								
								/* sinon si l'arc est à sens unique, on inverse ses règles de circulation */
								else if(nouvel_arc->vehicule == INDIFFERENT)
								{
									arc_inverse->vehicule = PIETON;
								}
									
								else
								{
									arc_inverse->vehicule = INDIFFERENT;
								}
								
								cpt_arcs += 2;
							}
						}
					}
				}
			}

		}
		
		if(chargement_reussi)
		{
			nb_arcs = cpt_arcs;
			printf("%d arcs chargés avec succès dans %s.\n", nb_arcs, nom_fichier);
		}
		
	}
	
	return chargement_reussi;
}



/***************************************************************************************************************/



/* cette fonction
- assigne à un arc reçu en paramètre son noeud de départ et son noeud d'arrivée à partir de leurs id reçus en paramètres
- insère cet arc dans la liste chaînée des arcs partant de son noeud de départ
- retourne 1 si l'insertion est réussie, 0 si elle a échoué
 */
int lier_arcs_noeuds(int id_noeud_depart, int id_noeud_arrivee, struct arc *a_inserer)
{
	struct noeud *noeud_depart, *noeud_arrivee;
	int insertion_reussie;
	
	insertion_reussie = 1;
	
	/* on récupère un pointeur sur le noeud de départ de l'arc */
	noeud_depart = acces_noeud(id_noeud_depart);

	/* message d'erreur si l'id_noeud_depart ne correspond à aucun noeud chargé */
	if(noeud_depart == NULL)
	{
		printf("Problème lors du chargement des arcs. noeud n° %d non trouvé.\n", id_noeud_depart);
		insertion_reussie = 0;
	}
	
	/* on récupère un pointeur sur le noeud d'arrivée de l'arc */
	noeud_arrivee = acces_noeud(id_noeud_arrivee);

	/* message d'erreur si l'id_noeud_arrivee ne correspond à aucun noeud chargé */
	if(noeud_arrivee == NULL)
	{
		printf("Problème lors du chargement des arcs. noeud n° %d non trouvé.\n", id_noeud_arrivee);
		insertion_reussie = 0;
	}
	
	/* si on a bien récupéré les deux pointeurs vers les noeuds de départ et d'arrivée */
	if(insertion_reussie)
	{
		/* insertion au début de la liste des arcs partant de noeud_depart */
		a_inserer->p_successeur = noeud_depart->debut_liste_arcs;
		noeud_depart->debut_liste_arcs = a_inserer;
		
		/* assignation à l'arc des pointeurs vers les deux noeuds */
		a_inserer->p_noeud_depart = noeud_depart;
		a_inserer->p_noeud_arrivee = noeud_arrivee;
	}
	
	return insertion_reussie;
	
}





/*****************************************************************************************/





/* procédure appelée avant un nouveau chargement ou l'arrêt du programme pour libérer l'espace alloué 
dynamiquement aux voies, noeuds et arcs */
void liberer_memoire()
{
	struct voie *voie_a_liberer, *voie_temp;
	struct noeud *noeud_a_liberer, *noeud_temp;
	struct arc *arc_a_liberer, *arc_temp;
	
	/* parcours de la liste des voies et libération de l'espace voie par voie */
	voie_temp = premiere_voie;
	while(voie_temp != NULL)
	{
		voie_a_liberer = voie_temp;
		voie_temp = voie_temp->succ_liste_globale;
		free(voie_a_liberer);
		voie_a_liberer = NULL;
	}
	
	/* parcours de la liste des noeuds */
	noeud_temp = premier_noeud;
	while(noeud_temp != NULL)
	{
		/* parcours de la liste des arcs partant de chaque noeud */
		arc_temp = noeud_temp->debut_liste_arcs;
		while(arc_temp != NULL)
		{
			arc_a_liberer = arc_temp;
			arc_temp = arc_temp->p_successeur;
			free(arc_a_liberer);
			arc_a_liberer = NULL;
		}
		
		noeud_a_liberer = noeud_temp;
		noeud_temp = noeud_temp->succ_liste_globale;
		free(noeud_a_liberer);
		noeud_a_liberer = NULL;
	}
	
	/* réinitialisation des variables globales */
	premiere_voie = NULL;
	derniere_voie = NULL;
	premier_noeud = NULL;
	dernier_noeud = NULL;
	nb_voies = 0;
	nb_noeuds = 0;
	nb_arcs = 0;
}
