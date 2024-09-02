#ifndef CHOIX_H
#define CHOIX_H

extern struct voie *choisir_voie();
extern struct voie *choisir_voie_cherchee();
extern struct noeud *choisir_noeud_voie(struct voie *p_voie);
extern struct noeud *choisir_noeud();

#endif