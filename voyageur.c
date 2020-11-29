#include <stdlib.h>
#include <stdio.h>
#include <time.h>
// #include <unistd.h>
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
  monvoyageur->couleur = YELLOW;
  // printf("\033[%d;%dH%c", monvoyageur->posX, monvoyageur->posY, monvoyageur->carvoy);
  return monvoyageur;
}

VOYAGEUR init_voyageurInd(char car, GARE magare, int tempsAttente, char entree){
  VOYAGEUR voyageurInd = (VOYAGEUR)malloc(sizeof(struct voyageur));
  // srand(time(NULL));
  if(entree == 0){
    if (rand()%2 == 0) {
      voyageurInd->quai = 'h';
      voyageurInd->posX = 0;
    }
    else {
      voyageurInd->quai ='b';
    }
    if (rand()%2 == 0) {
      if (voyageurInd->quai == 'h') {
        voyageurInd->posY = 33;
      }
      else {
        voyageurInd->posY = 0;
        voyageurInd->posX = 24;
      }
    }
    else {
      if (voyageurInd->quai == 'h') {
        voyageurInd->posY = 93;
      }
      else {
        voyageurInd->posY = 124;
        voyageurInd->posX = 24;
      }
    }
    //détermination des positions d'attente du train
    if(voyageurInd->posX ==0 && voyageurInd->posY > 62){
      voyageurInd->destinationY = rand()%60+62;
      voyageurInd->destinationX = rand()%8+2;
    } else if(voyageurInd->posX ==0 && voyageurInd->posY < 62){
      voyageurInd->destinationY = rand()%60+2;
      voyageurInd->destinationX = rand()%8+2;
    } else if(voyageurInd->posX ==24 && voyageurInd->posY > 62){
      voyageurInd->destinationY = rand()%60+62;
      voyageurInd->destinationX = rand()%8+20;
    } else if(voyageurInd->posX ==24 && voyageurInd->posY < 62){
      voyageurInd->destinationY = rand()%60+2;
      voyageurInd->destinationX = rand()%8+20;
    }
    // printf("\033[%d;%dH%c", voyageurInd->posX, voyageurInd->posY, voyageurInd->carvoy);
    voyageurInd->carpos = ' ';
    voyageurInd->tempsAttente = tempsAttente + 1 + rand()%5;
    voyageurInd->couleur = GREEN;
  } else if(entree == 'h'){
    voyageurInd->posX = 10; //lportehaut
    int nbporte =rand()%29+1;
    int coY=0;
    while(nbporte > 0){
      coY++;
      if(magare.custom[11][coY] == 'd'){
        nbporte--;
      }
    }
    voyageurInd->posY = coY;
    voyageurInd->destinationX = -1;
    if(voyageurInd->posY>62){voyageurInd->destinationY = 93;}
    else{voyageurInd->destinationY = 33;}
    voyageurInd->tempsAttente = tempsAttente;
    voyageurInd->carpos = '_';
    voyageurInd->couleur = BLUE;
  } else {
    voyageurInd->posX = 18; //lportehaut
    int nbporte =rand()%29+1;
    int coY=0;
    while(nbporte > 0){
      coY++;
      if(magare.custom[17][coY] == 'f'){
        nbporte--;
      }
    }
    voyageurInd->posY = coY;
    voyageurInd->destinationX = 23;
    if(voyageurInd->posY>62){voyageurInd->destinationY = 125;}
    else{voyageurInd->destinationY = -1;}
    voyageurInd->tempsAttente = tempsAttente;
    voyageurInd->carpos = '_';
    voyageurInd->couleur = BLUE;
  }
  voyageurInd->carvoy = car;
  magare.custom[voyageurInd->posX][voyageurInd->posY] = voyageurInd->carvoy;
  return voyageurInd;

}

void afficher_voyageur(VOYAGEUR monvoyageur){
  printf("\033[%d;%dH%c", monvoyageur->posX, monvoyageur->posY, monvoyageur->carvoy);
}

char mvtVoy(VOYAGEUR monvoyageur, GARE magare, char mvt){
  char posDep = 0;
  int posX;
  int posY;
  // printf("%d\n", monvoyageur.posX);
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
    default:
    return 0;
  }
  if (posX < 0 || posY < 0 || posY > 124) {
      printf("\033[%d;%dH%s", monvoyageur->posX+1, monvoyageur->posY+1, " ");
      magare.custom[monvoyageur->posX][monvoyageur->posY] = monvoyageur->carpos;
      monvoyageur->carpos = 'd';
      monvoyageur->posX = posX;
      monvoyageur->posY = posY;
      return 1;
    }
  posDep = magare.custom[posX][posY];
  if (posDep == ' ' || posDep == '_') {
    // printf("coucou\n");
    if(monvoyageur->carpos== ' '){
      printf("\033[%d;%dH%c", monvoyageur->posX+1, monvoyageur->posY+1, ' ');
    }
    else if(monvoyageur->carpos== '_'){
      printf("\033[%d;%dH%s%s%s", monvoyageur->posX+1, monvoyageur->posY+1, GREY, "■", DEFAULT_COLOR);
    }
    magare.custom[monvoyageur->posX][monvoyageur->posY] = monvoyageur->carpos;
    monvoyageur->carpos = posDep;
    monvoyageur->posX = posX;
    monvoyageur->posY = posY;
    magare.custom[monvoyageur->posX][monvoyageur->posY] = monvoyageur->carvoy;
    printf("\033[%d;%dH%s%c%s", monvoyageur->posX+1, monvoyageur->posY+1, monvoyageur->couleur, monvoyageur->carvoy, DEFAULT_COLOR);
    return 1;
  }
  else if (posDep == 'f' || posDep == 'd') {
    printf("\033[%d;%dH%s%s%s", monvoyageur->posX+1, monvoyageur->posY+1, GREY, "■", DEFAULT_COLOR);
    magare.custom[monvoyageur->posX][monvoyageur->posY] = monvoyageur->carpos;
    monvoyageur->carpos = posDep;
    monvoyageur->posX = posX;
    monvoyageur->posY = posY;
    return 1;
  }
  return 0;
}
