#ifndef H_TRAIN
#define H_TRAIN

#include <stdio.h>
#include <stdlib.h>


typedef struct train TRAIN;
struct train{
    char direction ;    /*N => Nord, S => Sud, E => EST, O => OUEST*/
    int posx;           /*Position courante x de la tête du train*/
    int posy;           /*Position courante y de l'arrière du train*/
    int vitesse;        /*Vitesse du train*/
    int lPortes;
    char ** custom;   /*Contient le train customisé, il faut choisir la bonne taille de votre tableau*/
    char etat;          /*État du train => dehors, entrant, stationné, sortant, sorti*/
    int portes;        /*Portes ouvertes ou fermées*/
    int longueur;       /*Longueur du train*/
    int tempsAttente;
    int tempsAQuai;
};

TRAIN init_train(FILE * fichier, char direction, int tempsAttente);

void afficherCarTrain(char c, int x, int y);

#endif
