#include <stdlib.h>
#include <stdio.h>
#include "gare.h"
#include "voyageurind.h"

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

INDVOYAGEUR init_voyageurInd(int car, GARE magare, char destination){
  INDVOYAGEUR voyageurInd = (INDVOYAGEUR)malloc(sizeof(struct voyageurInd));
  if (destination == 't'){
    voyageurInd->posX = 0;
    voyageurInd->posY = 31;
    voyageurInd->destinationX = 11;
    voyageurInd->destinationY = 40;
    voyageurInd->quai = 'h';
  } else if (destination == 'r') {
    voyageurInd->posX = 0;
    voyageurInd->posY = 93;
    voyageurInd->destinationX = 11;
    voyageurInd->destinationY = 99;
    voyageurInd->quai = 'h';
  } else if (destination == 'g') {
    voyageurInd->posX = 24;
    voyageurInd->posY = 0;
    voyageurInd->destinationX = 19;
    voyageurInd->destinationY = 40;
    voyageurInd->quai = 'b';
  } else if (destination == 'd') {
    voyageurInd->posX = 24;
    voyageurInd->posY = 124;
    voyageurInd->destinationX = 19;
    voyageurInd->destinationY = 99;
    voyageurInd->quai = 'b';
  }
  voyageurInd->carvoy = car;
  voyageurInd->carpos = ' ';
  magare.custom[voyageurInd->posX][voyageurInd->posY] = voyageurInd->carvoy;
  //détermination des positions d'attente du train
  if(voyageurInd->posX ==0 && voyageurInd->posY > 62){
    voyageurInd->attenteY = rand()%60+62;
    voyageurInd->attenteX = rand()%9+2;
  } else if(voyageurInd->posX ==0 && voyageurInd->posY < 62){
    voyageurInd->attenteY = rand()%60+2;
    voyageurInd->attenteX = rand()%9+2;
  } else if(voyageurInd->posX ==24 && voyageurInd->posY > 62){
    voyageurInd->attenteY = rand()%60+62;
    voyageurInd->attenteX = rand()%9+19;
  } else if(voyageurInd->posX ==24 && voyageurInd->posY < 62){
    voyageurInd->attenteY = rand()%60+2;
    voyageurInd->attenteX = rand()%9+19;
  }
  return voyageurInd;
}

void afficher_voyageurInd(INDVOYAGEUR voyageurInd){
  printf("\033[%d;%dH%c", voyageurInd->posX, voyageurInd->posY, voyageurInd->carvoy);
}

char mvtAleatoireVoy(INDVOYAGEUR voyageurInd, int aQuai){
  if(aQuai == 0){
    if(voyageurInd->posY == voyageurInd->destinationY && voyageurInd->quai == 'h'){
      return 's';
    } else if(voyageurInd->posY == voyageurInd->destinationY && voyageurInd->quai == 'b'){
      return 'z';
    } else if(voyageurInd->posY > voyageurInd->destinationY){
      return 'q';
    } else if(voyageurInd->posY < voyageurInd->destinationY){
      return 'd';
    }
  } else{
    if(voyageurInd->posY == voyageurInd->attenteY && voyageurInd->quai == 'h'){
      if(voyageurInd->posX == voyageurInd->attenteX){return 'r';}
      else if(voyageurInd->posX < voyageurInd->attenteX){return 's';}
      else {return 'z';}
    } else if(voyageurInd->posY == voyageurInd->attenteY && voyageurInd->quai == 'b'){
      if(voyageurInd->posX == voyageurInd->attenteX){return 'r';}
      else if(voyageurInd->posX < voyageurInd->attenteX){return 's';}
      else {return 'z';}
    } if(voyageurInd->posX == 0){
      return 's';
    } else if(voyageurInd->posY > voyageurInd->attenteY){
      return 'q';
    } else if(voyageurInd->posY < voyageurInd->attenteY){
      return 'd';
    }
  }
}

void mvtVoyInd(INDVOYAGEUR voyageurInd, GARE magare, char mvt){//faire bouger le voy suivant sa destination
  char posDep = 0;
  int posX;
  int posY;
  switch (mvt){
    case 'z' :
    posDep = magare.custom[voyageurInd->posX-1][voyageurInd->posY];
    posX = voyageurInd->posX-1;
    posY = voyageurInd->posY;
    // printf("haut\n");
    break;
    case 'q' :
    posDep = magare.custom[voyageurInd->posX][voyageurInd->posY-1];
    posX = voyageurInd->posX;
    posY = voyageurInd->posY-1;
    // printf("gauche\n");
    break;
    case 's' :
    posDep = magare.custom[voyageurInd->posX+1][voyageurInd->posY];
    posX = voyageurInd->posX+1;
    posY = voyageurInd->posY;
    break;
    case 'd' :
    posDep = magare.custom[voyageurInd->posX][voyageurInd->posY+1];
    posX = voyageurInd->posX;
    posY = voyageurInd->posY+1;
    break;
    case 'e':
    if(voyageurInd->carpos== ' '){
      printf("\033[%d;%dH%c", voyageurInd->posX+1, voyageurInd->posY+1, ' ');
    } else if(voyageurInd->carpos== '_'){
      printf("\033[%d;%dH%s%s%s", voyageurInd->posX+1, voyageurInd->posY+1, GREY, "■", DEFAULT_COLOR);
    }
    return;
    case 'r':
    return;
  }
  if (posDep == ' ' || posDep == '_') {
    if(voyageurInd->carpos== ' '){
      printf("\033[%d;%dH%c", voyageurInd->posX+1, voyageurInd->posY+1, ' ');
    } else if(voyageurInd->carpos== '_'){
      printf("\033[%d;%dH%s%s%s", voyageurInd->posX+1, voyageurInd->posY+1, GREY, "■", DEFAULT_COLOR);
    }
    magare.custom[voyageurInd->posX][voyageurInd->posY] = voyageurInd->carpos;
    voyageurInd->carpos = posDep;
    voyageurInd->posX = posX;
    voyageurInd->posY = posY;
    magare.custom[voyageurInd->posX][voyageurInd->posY] = voyageurInd->carvoy;
    printf("\033[%d;%dH%s%c%s", voyageurInd->posX+1, voyageurInd->posY+1, PURPLE, voyageurInd->carvoy, DEFAULT_COLOR);
  }
}
