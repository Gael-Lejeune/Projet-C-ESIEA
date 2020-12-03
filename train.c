#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "train.h"
#include "utils.h"


TRAIN init_train(FILE * fichier, char direction, int tempsAttente){
	fseek(fichier, 0, 0); //Retour au début du fichier contenant le train

	TRAIN montrain; //Declaration du train
	montrain.direction = direction; //'o' pour ouest et 'e' pour est
	if (direction == 'e') { //Si le train va vers l'est
		montrain.posx=0; //Position de la tete du train
		montrain.lPortes = 17; //Position des portes
	}
	else {
		montrain.posx=LONGUEUR+1;
		montrain.lPortes = 11;
	}
	montrain.longueur=LONGT; //Longueur du train
	montrain.vitesse= VITESSETRAIN; //Vitesse des trains
	montrain.etat='d'; //Dehors
	montrain.portes=0; //Fermées
	montrain.tempsAttente=tempsAttente; //Temps d'attente avant l'entrée en gare
	montrain.tempsAQuai=TEMPSAQUAI; //Temps à attendre en quai
	montrain.vide='v'; //Train vide
	montrain.custom = (char **)malloc(LARGT*sizeof(char *)); //Matrice du train
	for(int i =0; i<LARGT;i++) { //Chargement de la matrice à partir du fichier
		montrain.custom[i] = (char *)malloc(LONGT*sizeof(char));
	}
	char c;
	for(int i=0; i<LARGT; i++){
		for(int j=0; j<LONGT; j++){
			c = fgetc(fichier);
			montrain.custom[i][j] = c;
		}
		fgetc(fichier);
	}
	return montrain;
} //init_train()

void afficherCarTrain(char c, int x, int y){ //Afficher le caractere du train en ascii etendu à partir de l'ascii non etendu
	x += 1;
	y += 1;
	switch (c) {
		case 'a': //Tête ou queue du train
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "⎛", DEFAULT_COLOR);
		break;
		case 'w': //Tête ou queue du train
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "⎝", DEFAULT_COLOR);
		break;
		case '-': //Corps du train
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "▔", DEFAULT_COLOR);
		break;
		case '=': //Corps du train
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "▁", DEFAULT_COLOR);
		break;
		case '|': //Corps du train
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "|", DEFAULT_COLOR);
		break;
		case 'e': //Tête ou queue du train
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "⎞", DEFAULT_COLOR);
		break;
		case 'c': //Tête ou queue du train
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "⎠", DEFAULT_COLOR);
		break;
		case 'l': //Corps du train
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "⎣", DEFAULT_COLOR);
		break;
		case 'j': //Corps du train
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "⎦", DEFAULT_COLOR);
		break;
		case 'u': //Corps du train
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "⎤", DEFAULT_COLOR);
		break;
		case 'o': //Corps du train
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "⎡", DEFAULT_COLOR);
		break;
		case 'd': //Porte
		printf("\033[%d;%dH%s%s%s\n", x, y,DOORCOLOR, "-", DEFAULT_COLOR);
		break;
		case 'f': //Porte
		printf("\033[%d;%dH%s%s%s\n", x, y,DOORCOLOR, "-", DEFAULT_COLOR);
		break;
		default: //Si le caractère n'est pas reconnu
		printf("\033[%d;%dH%c\n", x, y, c);
		break;
	}
} //afficherCarTrain()
