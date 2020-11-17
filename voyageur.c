#include <stdlib.h>
#include <stdio.h>
#include "gare.h"
#include "voyageur.h"

#define LONGUEUR 126
#define LARGEUR 29

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

VOYAGEUR init_voyageur(int posX, int posY, int car, GARE magare){
    VOYAGEUR monvoyageur = (VOYAGEUR)malloc(sizeof(struct voyageur));
    monvoyageur->posX = posX;
    monvoyageur->posY = posY;
    monvoyageur->carvoy = car;
    monvoyageur->carpos = ' ';
    magare.custom[monvoyageur->posX][monvoyageur->posY] = monvoyageur->carvoy;
    return monvoyageur;
}

void afficher_voyageur(VOYAGEUR monvoyageur){
    printf("\033[%d;%dH%c", monvoyageur->posX, monvoyageur->posY, monvoyageur->carvoy);
}


void mvtVoy(VOYAGEUR monvoyageur, GARE magare, char mvt){
    char posDep = 0;
    int posX;
    int posY;
    // printf("%d\n", monvoyageur.posX);
    // printf("coucou\n");
    // mvt = 'z';
    switch (mvt){
        case 'z' :
        posX = monvoyageur->posX-1;
        posY = monvoyageur->posY;
        // printf("haut\n");
        break;
        case 'q' :
        posX = monvoyageur->posX;
        posY = monvoyageur->posY-1;
        // printf("gauche\n");
        break;
        case 's' :
        posX = monvoyageur->posX+1;
        posY = monvoyageur->posY;
        break;
        case 'd' :
        posX = monvoyageur->posX;
        posY = monvoyageur->posY+1;
        break;
    }
    if (posDep == ' ' || posDep == '_') {
      if(posX == 0 || posY == 0 || posY == 124){ //positions de sortie de gare
        printf("\033[%d;%dH%c", monvoyageur->posX+1, monvoyageur->posY+1, ' ');
        monvoyageur->carpos = posDep;
        magare.custom[monvoyageur->posX][monvoyageur->posY] = monvoyageur->carpos;
        printf("Bravo, vous êtes sorti !");
      } else{
        if(monvoyageur->carpos== ' '){
          printf("\033[%d;%dH%c", monvoyageur->posX+1, monvoyageur->posY+1, ' ');
        } else if(monvoyageur->carpos== '_'){
          printf("\033[%d;%dH%s%s%s", monvoyageur->posX+1, monvoyageur->posY+1, GREY, "■", DEFAULT_COLOR);
        }
        magare.custom[monvoyageur->posX][monvoyageur->posY] = monvoyageur->carpos;
        monvoyageur->carpos = posDep;
        monvoyageur->posX = posX;
        monvoyageur->posY = posY;
        magare.custom[monvoyageur->posX][monvoyageur->posY] = monvoyageur->carvoy;
        printf("\033[%d;%dH%s%c%s", monvoyageur->posX+1, monvoyageur->posY+1, CYAN, monvoyageur->carvoy, DEFAULT_COLOR);
      }
    }
    // printf("Coordonees : %d %d\n", monvoyageur.posX, monvoyageur.posY );
    // afficher_voyageur(monvoyageur);
}

//Pour que le voyageur arrive par la porte en haut à gauche : 0 ; 31
//Pour que le voyageur arrive par la porte en haut à droite : 0 ; 93
//Pour que le voyageur arrive par la porte en bas à gauche : 24 ; 0
//Pour que le voyageur arrive par la porte en bas à droite : 24 ; 124
