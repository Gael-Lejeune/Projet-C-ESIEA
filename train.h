#ifndef H_TRAIN
#define H_TRAIN

#include <stdio.h>
#include <stdlib.h>

typedef struct train TRAIN;
struct train{
    char direction ;    /*n => Nord, s => Sud, e => EST, o => OUEST*/
    int posx;           /*Position courante x de la tête du train*/
    int posy;           /*Position courante y de l'arrière du train*/
    int vitesse;        /*Vitesse du train*/
    char ** custom;     /*Contient le train customisé, il faut choisir la bonne taille de votre tableau*/
    char etat;          /*État du train => d dehors, e entrant, s stationné, p partant*/
    int portes;         /*Portes ouvertes ou fermées*/
    int longueur;       /*Longueur du train*/
    int tempsAttente;
    int tempsAQuai;
};

TRAIN init_train(FILE * fichier, char direction);

void deplacementTrain(TRAIN montrain, int rail, int col, int time, int posX, int posY, int posArret);

void afficherSortTrain(TRAIN montrain, int rail, int posX, int premierwagon, int val, int i);

void entreeTrain(TRAIN montrain, int posX, int time);

void afficherSortTrain(TRAIN montrain, int rail, int posX, int premierwagon, int val, int i);

void sortieTrain(TRAIN montrain, int rail, int posX, int time, int vitesse, char direction);

void afficherCarTrain(char c, int x, int y);

#endif
