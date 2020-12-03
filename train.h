#ifndef H_TRAIN
#define H_TRAIN

#include <stdio.h>
#include <stdlib.h>


typedef struct train TRAIN;
struct train{
    char direction ;    //N => Nord, S => Sud, E => EST, O => OUEST
    int posx;           //Position courante x de la tête du train
    int vitesse;        //Vitesse du train
    int lPortes;        //Ligne des portes du train
    char ** custom;     //Contient le train customisé, il faut choisir la bonne taille de votre tableau
    char etat;          //État du train => dehors, entrant, stationné, sortant, sorti
    int portes;         //Portes ouvertes ou fermées
    int longueur;       //Longueur du train
    int tempsAttente;   //Temps d'attente du train avant d'arriver en gare
    int tempsAQuai;     //Temps que le train doit passer à quai
    char vide;          //v vide, p plein
};

TRAIN init_train(FILE * fichier, char direction, int tempsAttente);

void afficherCarTrain(char c, int x, int y);

#endif
