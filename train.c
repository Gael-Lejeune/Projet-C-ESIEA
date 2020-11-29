#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "train.h"
#define LARGT 3
#define LONGT 116
#define PURPLE          "\033[1;35m"
#define CYAN            "\033[1;36m"
#define TRAINCOLOR      PURPLE
#define DOORCOLOR      	CYAN
#define DEFAULT_COLOR   "\033[0;m"

TRAIN init_train(FILE * fichier, char direction, int tempsAttente){
	TRAIN montrain;
	montrain.direction = direction;
	if (direction == 'e') {montrain.posx=0;}
	else {montrain.posx=126;}
	if (direction == 'e') {
		montrain.posx=0;
		montrain.lPortes = 17;
	}
	else {
		montrain.posx=126;
		montrain.lPortes = 11;
	}
	montrain.longueur=LONGT;
	montrain.posy=montrain.posx-LONGT;
	montrain.vitesse= 5000000;
	montrain.etat='d'; //dehors
	montrain.portes=0; //fermées
	montrain.tempsAttente=tempsAttente;
	montrain.tempsAQuai=250;
	montrain.vide='v';
	montrain.custom = (char **)malloc(LARGT*sizeof(char *));
	for(int i =0; i<LARGT;i++) {
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

void afficherCarTrain(char c, int x, int y){
	x += 1;
	y += 1;
	switch (c) {
		case 'a':
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "⎛", DEFAULT_COLOR);
		break;
		case 'w':
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "⎝", DEFAULT_COLOR);
		break;
		case '-':
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "▔", DEFAULT_COLOR);
		break;
		case '=':
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "▁", DEFAULT_COLOR);
		break;
		case 'e':
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "⎞", DEFAULT_COLOR);
		break;
		case 'c':
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "⎠", DEFAULT_COLOR);
		break;
		case 'l':
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "⎣", DEFAULT_COLOR);
		break;
		case 'j':
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "⎦", DEFAULT_COLOR);
		break;
		case 'u':
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "⎤", DEFAULT_COLOR);
		break;
		case 'o':
		printf("\033[%d;%dH%s%s%s\n", x, y,TRAINCOLOR, "⎡", DEFAULT_COLOR);
		break;
		case 'd':
		printf("\033[%d;%dH%s%s%s\n", x, y,DOORCOLOR, "-", DEFAULT_COLOR);
		// printf("\033[%d;%dH%s%s%s\n", x, y,DOORCOLOR, "¯", DEFAULT_COLOR);
		break;
		case 'f':
		printf("\033[%d;%dH%s%s%s\n", x, y,DOORCOLOR, "-", DEFAULT_COLOR);
		break;
		default:
		printf("\033[%d;%dH%c\n", x, y, c);
		break;
	}
} //afficherCarTrain()
