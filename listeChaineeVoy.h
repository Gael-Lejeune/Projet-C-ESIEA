#ifndef H_LISTEVCHAINEEVOY
#define H_LISTECHAINEEVOY

#include <stdio.h>
#include <stdlib.h>
#include "voyageur.h"

typedef struct  elementV ELEMENTV;
struct elementV{
    VOYAGEUR voyageur;
    ELEMENTV *precedent;
    ELEMENTV *suivant; //permet de lier les éléments entre eux
};

typedef struct listeV LISTEV;
struct listeV
{
    ELEMENTV *premier;
    ELEMENTV *dernier;
};

void init_liste_voy(LISTEV *l); //Initialisation de la listeV chainée

void ajout_voy_fin(LISTEV *l, VOYAGEUR t); //ajout d'élément en fin de listeV

void ajout_voy_debut(LISTEV *l, VOYAGEUR t); //ajoute au début

void supp_voy_fin(LISTEV *l); //supprime le dernier

void supp_voy_debut(LISTEV *l); //supprime le premier

void supp_voy(LISTEV*l, ELEMENTV * element); //supprime un élément quelconque

void clear_voy(LISTEV *l); //Vide la listeV







#endif
