#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "gare.h"
#include "voyageur.h"
#include "utils.h"



VOYAGEUR init_voyageur(int posX, int posY, int car, GARE magare){
  //Initialise un voyageur
  VOYAGEUR monvoyageur = (VOYAGEUR)malloc(sizeof(struct voyageur));
  monvoyageur->posX = posX;
  monvoyageur->posY = posY;
  monvoyageur->carvoy = car;
  monvoyageur->carpos = ' ';
  magare.custom[monvoyageur->posX][monvoyageur->posY] = monvoyageur->carvoy;
  monvoyageur->couleur = PLAYERCOLOR;
  return monvoyageur;
}//init_voyageur()

VOYAGEUR init_voyageurInd(char car, GARE magare, int tempsAttente, char entree){
  VOYAGEUR voyageurInd = (VOYAGEUR)malloc(sizeof(struct voyageur));
  if(entree == 0){
    //Si le voyageur entre par le quai
    if (rand()%2 == 0) {
      //Une fois sur deux
      voyageurInd->quai = 'h'; //Créer en haut
      voyageurInd->posX = 0;
    }
    else {
      voyageurInd->quai ='b'; //Créer en bas
    }
    if (rand()%2 == 0) {
      //Une fois sur deux créer à droite
      if (voyageurInd->quai == 'h') {
        //Pour le quai en haut
        voyageurInd->posY = 33;
      }
      else {
        //Pour le quai en bas
        voyageurInd->posY = 0;
        voyageurInd->posX = 24;
      }
    }
    else {
      //Créer à gauche
      if (voyageurInd->quai == 'h') {
        voyageurInd->posY = 93;
      }
      else {
        voyageurInd->posY = LONGUEUR-1;
        voyageurInd->posX = 24;
      }
    }
    //Détermination d'une position aléatoire d'attente pour le train
    if(voyageurInd->posX ==0 && voyageurInd->posY > 59){
      //Si le voyageur est en haut à droite, attendre dans une portion du quai du haut à droite
      voyageurInd->destinationY = rand()%59+59;
      voyageurInd->destinationX = rand()%8+2;
    } else if(voyageurInd->posX ==0 && voyageurInd->posY < 59){
      voyageurInd->destinationY = rand()%59+2;
      voyageurInd->destinationX = rand()%8+2;
    } else if(voyageurInd->posX ==24 && voyageurInd->posY > 59){
      voyageurInd->destinationY = rand()%59+59;
      voyageurInd->destinationX = rand()%8+20;
    } else if(voyageurInd->posX ==24 && voyageurInd->posY < 59){
      voyageurInd->destinationY = rand()%59+2;
      voyageurInd->destinationX = rand()%8+20;
    }
    voyageurInd->carpos = ' ';
    voyageurInd->tempsAttente = tempsAttente + 1 + rand()%5; //Temps d'attente aléatoire
    voyageurInd->couleur = GREEN;
  }
  else if(entree == 'h'){
    //Si le voyageur sort du train en haut
    voyageurInd->posX = 10; //Portes du train du haut
    int nbporte =rand()%29+1; //Porte de sortie aléatoire
    int coY=0; //Col Y
    while(nbporte > 0){
      //Pour toutes les portes
      coY++;
      if(magare.custom[11][coY] == 'd'){
        nbporte--;
      }
    }
    voyageurInd->posY = coY; //Colonne de la porte tirée au hasard
    voyageurInd->destinationX = -1; //Position de sortie de la gare
    if(voyageurInd->posY>59){voyageurInd->destinationY = 93;} //Si le voyageur est à droite, sortie sur la porte de droite
    else{voyageurInd->destinationY = 33;} //Sinon, sortie sur la porte de gauche
    voyageurInd->tempsAttente = tempsAttente;
    voyageurInd->carpos = '_'; //Bord du quai
    voyageurInd->couleur = BLUE;
  }
  else {
    //Même processus pour le train d'en bas
    voyageurInd->posX = 18;
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
    if(voyageurInd->posY>59){voyageurInd->destinationY = 125;}
    else{voyageurInd->destinationY = -1;}
    voyageurInd->tempsAttente = tempsAttente;
    voyageurInd->carpos = '_';
    voyageurInd->couleur = BLUE;
  }
  voyageurInd->carvoy = car;
  magare.custom[voyageurInd->posX][voyageurInd->posY] = voyageurInd->carvoy;
  return voyageurInd;
}//init_voyageurInd()

void afficher_voyageur(VOYAGEUR monvoyageur){
  printf("\033[%d;%dH%c\n", monvoyageur->posX, monvoyageur->posY, monvoyageur->carvoy);
}//afficher_voyageur()

char mvt_voyageur(VOYAGEUR monvoyageur, GARE magare, char mvt){
  //Permet de déplacer le voyageur
  char posDep = 0; //Caractère de position de déplacement
  int posX;
  int posY;
  switch (mvt){
    case 'z' : //Si la direction est le haut
    posX = monvoyageur->posX-1;
    posY = monvoyageur->posY;
    break;
    case 'q' : //La gauche
    posX = monvoyageur->posX;
    posY = monvoyageur->posY-1;
    break;
    case 's' : //Le bas
    posX = monvoyageur->posX+1;
    posY = monvoyageur->posY;
    break;
    case 'd' : //La droite
    posX = monvoyageur->posX;
    posY = monvoyageur->posY+1;
    break;
    default: //Autre touche
    return 0;
  }
  if (posX < 0 || posY < 0 || posY > LONGUEUR-1) {
    //Si le voyageur atteint une position de sortie de gare
    printf("\033[%d;%dH%s\n", monvoyageur->posX+1, monvoyageur->posY+1, " "); //Le faire disparaitre
    magare.custom[monvoyageur->posX][monvoyageur->posY] = monvoyageur->carpos; //Supprimer de la matrice
    monvoyageur->carpos = 'd'; //Caractère de position "dehors"
    monvoyageur->posX = posX;
    monvoyageur->posY = posY;
    return 1;
  }
  posDep = magare.custom[posX][posY]; //Caractère de position de déplacement
  if (posDep == ' ' || posDep == '_') {
    //Si l'emplacement est vide ou le bord du quai
    if(monvoyageur->carpos== ' '){
      //Rétablir le caractère de la position précédente
      printf("\033[%d;%dH%c\n", monvoyageur->posX+1, monvoyageur->posY+1, ' ');
    }
    else if(monvoyageur->carpos== '_'){
      //Rétablir le caractère de la position précédente
      printf("\033[%d;%dH%s%s%s\n", monvoyageur->posX+1, monvoyageur->posY+1, GREY, "■", DEFAULT_COLOR);
    }

    magare.custom[monvoyageur->posX][monvoyageur->posY] = monvoyageur->carpos; //Replacer le caractère de la position précédente dans la matrice
    monvoyageur->carpos = posDep; //Caractère de la position de déplacement
    monvoyageur->posX = posX;
    monvoyageur->posY = posY;
    magare.custom[monvoyageur->posX][monvoyageur->posY] = monvoyageur->carvoy; //Placement du voyageur dans la matrice
    printf("\033[%d;%dH%s%c%s\n", monvoyageur->posX+1, monvoyageur->posY+1, monvoyageur->couleur, monvoyageur->carvoy, DEFAULT_COLOR); //Affichage du voyageur à sa nouvelle position
    return 1;
  }
  else if (posDep == 'f' || posDep == 'd') {
    //Si le caractère de déplacement est une porte
    printf("\033[%d;%dH%s%s%s\n", monvoyageur->posX+1, monvoyageur->posY+1, GREY, "■", DEFAULT_COLOR); //Afficher le bord du quai sur la position
    magare.custom[monvoyageur->posX][monvoyageur->posY] = monvoyageur->carpos; //Supprimer le voyageur de la matrice
    monvoyageur->carpos = posDep;
    monvoyageur->posX = posX;
    monvoyageur->posY = posY;
    return 1;
  }
  return 0; //Si le voyageur n'a pas pu se déplacer à cause d'un obstacle
}//mvt_voyageur()
