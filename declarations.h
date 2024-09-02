#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* définition des constantes */
#define MAX_NOM_FICHIER 50
#define MAX_NOM_VOIE 100
#define MAX_NOM_NOEUD 100

#define INDIFFERENT 0
#define PIETON 1
#define VOITURE 2

/* #######################################################
             DÉFINITION DES TYPES STRUCTURÉS
   -------------------------------------------------------
   trois types correspondant aux trois fichiers de données
   - les noeuds (croisements de rue)
   - les arcs qui relient les noeuds entre eux
   - les voies auxquelles appartiennent les arcs
   #######################################################
*/

struct voie{
	
	/* identifiant numérique unique et nom de la voie, chargés depuis le fichier des voies */
	int id_voie;
	char nom_voie[MAX_NOM_VOIE];
	
	/* pointeurs vers les éléments précédent et suivant dans la liste chaînée globale des voies */
	struct voie *pred_liste_globale;
	struct voie *succ_liste_globale;
};

struct noeud{
	
	/* identifiant numérique unique et nom du noeud, chargés depuis le fichier des noeuds */
	int id_noeud;
	char nom_noeud[MAX_NOM_NOEUD];
	
	/* pointeurs vers les éléments précédent et suivant dans la liste chaînée globale des noeuds */
	struct noeud *pred_liste_globale;
	struct noeud *succ_liste_globale;
	
	/* pointeur vers le début de la liste chaînée d'arcs partant du noeud, remplie au moment du chargement du fichier des arcs */
	struct arc *debut_liste_arcs;
	
	/* variables utilisées lors de l'exécution de l'algorithme :
		1. pointeurs vers les noeuds précédent et suivant dans la liste chaînée des noeuds ajoutés au sous-graphe
		2. pointeur vers l'arc dont provient le chemin le plus court jusqu'au noeud
		3. distance totale la plus courte au noeud de départ */
	struct noeud *pred_liste_dijkstra;
	struct noeud *succ_liste_dijkstra;
	struct arc *p_provenance;
	int distance_au_depart;
};

struct arc{
	
	/* variables initialisées au chargement du fichier des arcs */
	
	int longueur; // longueur en mètres de l'arc
	int vehicule; // est-il piéton ou non ? trois valeurs possibles :
				// 0 (constante INDIFFERENT) : peut être emprunté par les voitures et les piétons
				// 1 (constante PIETON) : ne peut être emprunté que par les piétons
				// 2 (constante VOITURE) : ne peut être emprunté que par les voitures
				
	/* pointeurs vers les noeuds de départ et d'arrivée de l'arc ainsi que vers sa voie, liés au moment du chargement du fichier des arcs */
	struct noeud *p_noeud_depart;
	struct noeud *p_noeud_arrivee;
	struct voie *p_voie_arc;
	
	/* pointeur vers le successeur de l'arc dans la liste chaînée des arcs partant de ce noeud */
	struct arc *p_successeur;
};

#endif