/* #######################################################
            SOUS-PROGRAMMES D'ACCES AUX DONNÉES
   #######################################################
*/



#include "acces.h"
#include "declarations.h"



extern struct voie *premiere_voie;
extern struct noeud *premier_noeud;
extern int nb_voies;
extern int nb_noeuds;



/* fonction qui prend en paramètre un entier et retourne un pointeur sur la voie dont c'est l'identifiant
retourne NULL si la voie correspondante n'est pas trouvée */
struct voie *acces_voie(int id)
{
	struct voie *voie_cherchee;
	int cpt;
	
	/* on vérifie que l'ID de la voie cherchée est valide */
	if((id < 0) || (id >= nb_voies))
	{
		voie_cherchee = NULL;
	}
	
	else
	{
		/* positionnement au début de la liste */
		voie_cherchee = premiere_voie;
		
		/* on parcourt la liste autant de fois que nécessaire (les voies sont classées par ordre croissant de leur identifiant numérique) */
		for(cpt = 0 ; cpt < id ; cpt++)
		{
			voie_cherchee = voie_cherchee->succ_liste_globale;
		}
	}
	
	/* on retourne voie_cherchee qui vaut NULL si la voie n'a pas été trouvée */
	return voie_cherchee;
}



/***************************************************************************************************************/



/* fonction qui prend en paramètre un entier et retourne un pointeur sur le noeud dont c'est l'identifiant
retourne NULL si le noeud correspondant n'est pas trouvé */
struct noeud *acces_noeud(int id)
{
	struct noeud *noeud_cherche;
	int cpt;
	
	/* on vérifie que l'ID du noeud cherché est valide */
	if((id < 0) || (id >= nb_noeuds))
	{
		noeud_cherche = NULL;
	}
	
	else
	{
		/* positionnement au début de la liste */
		noeud_cherche = premier_noeud;
		
		/* on parcourt la liste autant de fois que nécessaire (les noeuds sont classés par ordre croissant de leur identifiant numérique) */
		for(cpt = 0 ; cpt < id ; cpt++)
		{
			noeud_cherche = noeud_cherche->succ_liste_globale;
		}
	}
	
	/* on retourne noeud_cherche qui vaut NULL si le noeud n'a pas été trouvé */
	return noeud_cherche;
}
