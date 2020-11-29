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
    char carpos; //contenu de la case sur laquelle le voyageur est allé
    int destinationX;
    int destinationY;
    char quai; //h ou b
    int tempsAttente;
    char* couleur;
};

VOYAGEUR init_voyageur(int posX, int posY, int car, GARE magare);

VOYAGEUR init_voyageurInd(char car, GARE magare, int tempsAttente, char entree);
//entree = 0 pour Voy entrant, h ou b pour Voy sortant du train

void afficher_voyageur(VOYAGEUR monvoyageur);

char mvtVoy(VOYAGEUR monvoyageur, GARE magare, char mvt);

#endif
