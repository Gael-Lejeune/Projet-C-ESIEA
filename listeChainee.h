#ifndef H_LISTECHAINEE
#define H_LISTECHAINEE

#include <stdio.h>
#include <stdlib.h>
#include "train.h"

typedef struct  element ELEMENT;
struct element{
    TRAIN train;
    ELEMENT *precedent;
    ELEMENT *suivant; //Permet de lier les éléments entre eux
};

typedef struct liste LISTE;
struct liste
{
    ELEMENT *premier;
    ELEMENT *dernier;
};

void init_liste_train(LISTE *l); //Initialisation de la liste chainée

void ajout_train_fin(LISTE *l, TRAIN t); //Ajout d'élément en fin de liste

void ajout_train_debut(LISTE *l, TRAIN t); //Ajoute au début

void supp_train_fin(LISTE *l); //Supprime le dernier

void supp_train_debut(LISTE *l); //Supprime le premier

void clear_train(LISTE *l); //Vide la liste







#endif
