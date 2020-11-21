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

void init_listeV(LISTEV *l); //Initialisation de la listeV chainée

void ajoutVF(LISTEV *l, VOYAGEUR t); //ajout d'élément en fin de listeV

void ajoutVD(LISTEV *l, VOYAGEUR t); //ajoute au début

void suppVF(LISTEV *l); //supprime le dernier

void suppVD(LISTEV *l); //supprime le premier

void ClearV(LISTEV *l); //Vide la listeV







#endif
