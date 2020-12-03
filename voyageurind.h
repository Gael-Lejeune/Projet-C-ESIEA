#ifndef H_INDVOYAGEUR
#define H_INDVOYAGEUR

#include <stdlib.h>
#include <stdio.h>
#include "gare.h"

typedef struct voyageurInd * INDVOYAGEUR;
struct voyageurInd{
    char carvoy;
    int posX;
    int posY;
    char carpos; //contenu de la case sur laquelle le voyageur est allé
    int destinationX;
    int destinationY;
    char quai; //h ou b
};

INDVOYAGEUR init_voyageurInd(int car, GARE magare, char destination);

char mvtAleatoireVoy(INDVOYAGEUR voyageurInd, int aQuai);

void afficher_voyageurInd(INDVOYAGEUR voyageurInd);

void mvtVoyInd(INDVOYAGEUR voyageurInd, GARE magare, char mvt);

#endif
