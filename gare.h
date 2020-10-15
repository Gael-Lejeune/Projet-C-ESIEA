#ifndef H_GARE
#define H_GARE

#include <stdio.h>
#include <stdlib.h>

typedef struct gare GARE;
struct gare{
    char ** custom;   /*Contient le train customisé, il faut choisirla bonne taille de votre tableau*/
};

char ** ALLOCATION_MAT_DYN(int NB_L, int NB_C);

GARE init_gare(FILE * fichier);

void afficher_gare (FILE * fichier,GARE magare);


#endif
