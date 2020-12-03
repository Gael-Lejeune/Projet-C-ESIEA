#ifndef H_VOYAGEUR
#define H_VOYAGEUR

#include <stdlib.h>
#include <stdio.h>
#include "gare.h"

typedef struct voyageur * VOYAGEUR;
struct voyageur{
    char carvoy;        //Caractère du voyageur
    char* couleur;      //Couleur du voyageur
    int posX;           //Position abscisse
    int posY;           //Position ordonnée
    char carpos;        //Contenu de la case sur laquelle le voyageur se trouve
    int destinationX;   //Abscisse de la destination
    int destinationY;   //Ordonnée de la destination
    char quai;          //"h" pour haut ou "b" pour bas
    int tempsAttente;   //Temps avant de rentrer dans la gare
};

VOYAGEUR init_voyageur(int posX, int posY, int car, GARE magare); //Initialise le voyageur du joueur

VOYAGEUR init_voyageurInd(char car, GARE magare, int tempsAttente, char entree); //Initialise les voyageurs indépendants
//entree = 0 pour Voy entrant, h ou b pour Voy sortant du train

void afficher_voyageur(VOYAGEUR monvoyageur); //Affiche le voyageur dans la matrice

char mvt_voyageur(VOYAGEUR monvoyageur, GARE magare, char mvt); //Deplace le voyageur dans la matrice

#endif
