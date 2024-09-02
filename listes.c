/* #######################################################
        SOUS-PROGRAMMES D'AFFICHAGE DE LISTES
   #######################################################
*/



#include "declarations.h"
#include "listes.h"



/* variables globales déclarées dans le fichier source principal */
extern struct voie *premiere_voie;
extern struct noeud *premier_noeud;
extern int nb_voies, nb_noeuds;




/* affichage formaté des voies (id_voie et nom_voie) dans l'ordre croissant de leur id */
void liste_voies()
{
	struct voie *voie_courante;
	
	if(nb_voies == 0)
	{
		printf("Aucune voie à afficher.\n\n");
	}
	
	else
	{
		/* en-tête du tableau */
		printf("---------------------------------------------------------------------\n");
		printf("%-20s|%17s\n", "NUMÉRO DE LA VOIE", "NOM DE LA VOIE");
		printf("---------------------------------------------------------------------\n");
		
		/* parcours et affichage de la liste de voies */
		voie_courante = premiere_voie;
		
		while(voie_courante != NULL)
		{
			printf("%17d   |   %s\n", voie_courante->id_voie, voie_courante->nom_voie);
			voie_courante = voie_courante->succ_liste_globale;
		}
		
		printf("---------------------------------------------------------------------\n\n\n");
		
	}
	
}



/*****************************************************************************************/



/* affichage formaté des noeuds (id_noeud et nom_noeud) dans l'ordre croissant de leur id */
void liste_noeuds()
{
	struct noeud *noeud_courant;
	
	if(nb_noeuds == 0)
	{
		printf("Aucun noeud à afficher.\n");
	}
	
	else
	{
		/* en-tête du tableau */
		printf("---------------------------------------------------------------------\n");
		printf("%-19s|%16s\n", "NUMÉRO DU NOEUD", "NOM DU NOEUD");
		printf("---------------------------------------------------------------------\n");
		
		/* parcours et affichage de la liste des noeuds */
		noeud_courant = premier_noeud;
		
		while(noeud_courant != NULL)
		{
			printf("%16d   |   %s\n", noeud_courant->id_noeud, noeud_courant->nom_noeud);
			noeud_courant = noeud_courant->succ_liste_globale;
		}
		
		printf("---------------------------------------------------------------------\n\n\n");
	}
	
}
