#include <stdlib.h>
#include <stdio.h>
#include "gare.h"

#define LONGUEUR 101
#define LARGEUR 17

#define BLACK    "\033[1;30m"
#define RED      "\033[1;31m"
#define GREEN    "\033[1;32m"
#define YELLOW   "\033[1;33m"
#define BLUE     "\033[1;34m"
#define PURPLE   "\033[1;35m"
#define CYAN     "\033[1;36m"
#define GREY     "\033[1;37m"
#define DEFAULT_COLOR "\033[0;m"
// #define CARVOY  '*'

typedef struct voyageur VOYAGEUR;
struct voyageur{
    char carvoy;
    int posX;
    int posY;
};

VOYAGEUR init_voyageur(int posX, int posY, int car){
    VOYAGEUR monvoyageur;
    monvoyageur.posX = posX;
    monvoyageur.posY = posY;
    monvoyageur.carvoy = car;
    return monvoyageur;
}

void mvtVoy(VOYAGEUR monvoyageur, GARE magare, char mvt){
    int posDep;
    int posX;
    int posY;
    switch (mvt){
        case 'z' :
            posDep = magare.custom[monvoyageur.posX-1][monvoyageur.posY];
            posX = monvoyageur.posX-1;
            posY = monvoyageur.posY;
        break;
        case 'q' :
            posDep = magare.custom[monvoyageur.posX][monvoyageur.posY-1];
            posX = monvoyageur.posX;
            posY = monvoyageur.posY-1;
        break;
        case 's' :
            posDep = magare.custom[monvoyageur.posX+1][monvoyageur.posY];
            posX = monvoyageur.posX+1;
            posY = monvoyageur.posY;
        break;
        case 'd' :
            posDep = magare.custom[monvoyageur.posX][monvoyageur.posY+1];
            posX = monvoyageur.posX;
            posY = monvoyageur.posY+1;
        break;
    }

    if (posDep != ' ') {
        monvoyageur.posX = posX;
        monvoyageur.posY = posY;
        magare.custom[monvoyageur.posX][monvoyageur.posY] = ' ';
        magare.custom[monvoyageur.posX][monvoyageur.posY] = monvoyageur.carvoy;
    }
}
