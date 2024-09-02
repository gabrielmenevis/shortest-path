/* #######################################################
          SOUS-PROGRAMMES D'AFFICHAGE DES DONNÉES
   #######################################################
*/



#include "declarations.h"
#include "affichage.h"



/* cette procédure prend en paramètres deux chaînes de caractères et les affiche l'une à la suite de l'autre dans un
cadre dont la longueur est calculée selon une formule fixe
elle est destinée à prendre en paramètre comme première chaîne du type "Informations du noeud/de la voie" et comme deuxième chaîne le nom de l'élément concerné */
void affichage_encadre(char c1[], char c2[])
{
	int nb_tirets;
	int i;
	
	nb_tirets = strlen(c1) + strlen(c2) + 4;
	
	printf("\n\t\t ");
	for(i = 0 ; i < nb_tirets ; i++)
	{
		printf("-");
	}
	printf("\n\t\t");
	
	printf("/ %s %s /", c1, c2);
	
	printf("\n\t\t");
	for(i = 0 ; i < nb_tirets ; i++)
	{
		printf("-");
	}
	printf("\n\n");
	
}
