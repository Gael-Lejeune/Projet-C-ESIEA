#ifndef H_VOYAGEUR
#define H_VOYAGEUR

#include <stdlib.h>
#include <stdio.h>
#include "gare.h"

typedef struct voyageur * VOYAGEUR;
struct voyageur{
    char carvoy;
    int posX;
    int posY;
};

VOYAGEUR init_voyageur(int posX, int posY, int car, GARE magare);

void afficher_voyageur(VOYAGEUR monvoyageur);

void mvtVoy(VOYAGEUR monvoyageur, GARE magare, char mvt);

#endif
