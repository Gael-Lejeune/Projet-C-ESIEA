#ifndef H_LISTECHAINEE
#define H_LISTECHAINEE

#include <stdio.h>
#include <stdlib.h>
#include "train.h"

typedef struct  element ELEMENT;
struct element{
    TRAIN train;
    ELEMENT *precedent;
    ELEMENT *suivant; //permet de lier les éléments entre eux
};

typedef struct liste LISTE;
struct liste
{
    ELEMENT *premier;
    ELEMENT *dernier;
};

void init_liste(LISTE *l); //Initialisation de la liste chainée

void ajoutF(LISTE *l, TRAIN t); //ajout d'élément en fin de liste

void ajoutD(LISTE *l, TRAIN t); //ajoute au début

void suppF(LISTE *l); //supprime le dernier

void suppD(LISTE *l); //supprime le premier

void View(LISTE l);

void Clear(LISTE *l); //Vide la liste







#endif
