#ifndef H_TRAIN
#define H_TRAIN

#include <stdio.h>
#include <stdlib.h>


typedef struct train TRAIN;
struct train{
    char direction ;    //E => EST, O => OUEST
    int posx;           //Position courante x de la tête du train
    int vitesse;        //Vitesse du train
    int lPortes;        //Ligne des portes du train
    char ** custom;     //Contient la matrice du train
    char etat;          //État du train => dehors, entrant, stationné, partant
    int portes;         //Portes ouvertes ou fermées
    int longueur;       //Longueur du train
    int tempsAttente;   //Temps d'attente du train avant d'arriver en gare
    int tempsAQuai;     //Temps que le train doit passer à quai
    char vide;          //v vide, p plein
};

TRAIN init_train(FILE * fichier, char direction, int tempsAttente); //Initialisation du train

void afficherCarTrain(char c, int x, int y); //Affichage du train

#endif
