#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "train.h"
#include "mouvTrain.c"


void mvtTrainEnGare(struct TRAIN montrain){
	int time=0;
	int posX, posY;
	int rail = '_';
	int tempsAQuai = 100;
	if (montrain.direction == 'e'){
		posX = 7;
		posY = 0;
		entreeTrain(montrain, posX, time);
		deplacementTrain(montrain, rail, 0, time, posX, posY, 13);
		while(tempsAQuai > 0){
			montrain.portes = 1;
			tempsAQuai --;
		}
		montrain.portes = 0;
		deplacementTrain(montrain, rail, 12, time, posX, posY, 24);
		sortieTrain(montrain, rail, posX, time, montrain.vitesse, 'e');
	}
	else{
		posX = 10;
		posY = 0;
		entreeTrain(montrain, posX, time);
		sleep(2);
		deplacementTrain(montrain, rail, 22, time, posX+3, posY, 12);
		while(tempsAQuai > 0){
			montrain.portes = 1;
			tempsAQuai --;
		}
		montrain.portes = 0;
		deplacementTrain(montrain, rail, 12, time, posX+3, posY, 0);
		sortieTrain(montrain, rail, posX+3, time, montrain.vitesse, montrain.direction);
		montrain.etat = 's';
	}
}


void mvtTrain(struct TRAIN T, int tempsAttente){
	while(1){
		switch (T.etat){
			case 'd':
				if (tempsAttente<=2){
					T.etat='e';
				}
				tempsAttente--;
				break;
			case 'e':
				mvtTrainEnGare(T);
			case 's' :
				tempsAttente=5;
				printf("fin");
				return;
		}
	}
}
#define largT 2
#define longT 52
/*struct TRAIN init_train(FILE * fichier){
	struct TRAIN T;
	T.custom = ALLOCATION_MAT_DYN(largT,longT);
	char c;
	for(int i=0; i<largT; i++){
		for(int j=0; j<longT; j++){
			c = fgetc(fichier);
			if (c == -1000) {
				T.custom[i][j] = '1';
			}
			else {
				T.custom[i][j] = 'w';
			}
		}
	}
	T.direction = 'o';
	T.posx=7;
	T.posy=0;
	T.vitesse= 3000;
	T.etat='d'; //dehors
	T.portes=0; //fermées
	T.longueur=52;
    return T;
}*/



int main(){
	//int tempsAttente=3;
	FILE * train = fopen("txt/train_test.txt", "r");
	struct TRAIN T = init_train(train);
	mvtTrain(T, 1);
	//afficher_train (train, T);
	return 0;
}
