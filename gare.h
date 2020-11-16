#ifndef H_GARE
#define H_GARE

#include <stdio.h>
#include <stdlib.h>

typedef struct gare GARE;
struct gare{
    char ** custom;   /*Contient le train customisé, il faut choisirla bonne taille de votre tableau*/
};

GARE init_gare(FILE * fichier); //Initialisation de la gare via fichier txt

void afficher_gare (GARE magare); //affichage de la gare

#endif
