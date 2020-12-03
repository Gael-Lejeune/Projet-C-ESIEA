#ifndef H_GARE
#define H_GARE

#include <stdio.h>
#include <stdlib.h>

typedef struct gare GARE;
struct gare{
    char ** custom; //Matrice stockant les caractères de la gare
};

GARE init_gare(char * fichier); //Initialisation de la gare via fichier txt

void afficher_gare (GARE magare); //Affichage de la gare

#endif
