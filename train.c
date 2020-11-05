#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "train.h"

#define LARGT 2
#define LONGT 52

TRAIN init_train(FILE * fichier, char direction){
	TRAIN montrain;
	montrain.direction = direction;
	if (direction == 'e') {montrain.posx=0;}
	else {montrain.posx=63;}
	montrain.posy=0;
	montrain.vitesse= 10000000;
	montrain.etat='d'; //dehors
	montrain.portes=0; //fermées
	montrain.longueur=LONGT;

	montrain.custom = (char **)malloc(2*sizeof(char *));
	for(int i =0; i<2;i++) {
		montrain.custom[i] = (char *)malloc(LONGT*sizeof(char));
	}

	char c;
	for(int i=0; i<2; i++){
		for(int j=0; j<LONGT; j++){
			c = fgetc(fichier);
			montrain.custom[i][j] = c;
		}
		fgetc(fichier);
	}
	return montrain;
}

void mvtTrain(TRAIN montrain, int tempsAttente){
	int time=0;
	int posX, posY;
	int rail = '-';
	int tempsAQuai = 100;
	while(1){
		switch (montrain.etat){
			case 'd':
			if (tempsAttente<=2){
				montrain.etat='e';
			}
			tempsAttente--;
			break;
			case 'e':
			if (montrain.direction == 'e'){
				posX = 10;
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
				posX = 7;
				posY = 0;
				entreeTrain(montrain, posX, time);
				sleep(2);
				deplacementTrain(montrain, rail, 22, time, posX, posY, 12);
				while(tempsAQuai > 0){
					montrain.portes = 1;
					tempsAQuai --;
				}
				montrain.portes = 0;
				deplacementTrain(montrain, rail, 12, time, posX, posY, 0);
				sortieTrain(montrain, rail, posX, time, montrain.vitesse, montrain.direction);
				montrain.etat = 's';
			}
			break;
			case 's' :
			tempsAttente=5;
			printf("fin");
		}
	}
}

void deplacementTrain(TRAIN montrain, int rail, int col, int time, int posX, int posY, int posArret){
	if (montrain.direction == 'e'){
		while(col<posArret){
			time=(time+1)%montrain.vitesse;
			if(time==1000){
				posY=col;
				printf("\033[%d;%dH%c", posX, posY-1, rail);
				printf("\033[%d;%dH%s\n", posX, posY, montrain.custom[0]);
				printf("\033[%d;%dH%c", posX+1, posY-1, rail);
				printf("\033[%d;%dH%s\n", posX+1, posY, montrain.custom[1]);
				col++;
			}
		}
	}
	else {
		while(col>posArret){
			time=(time+1)%montrain.vitesse;
			if(time==1000){
				posY=col;
				printf("\033[%d;%dH%s\n", posX, posY, montrain.custom[0]);
				printf("\033[%d;%dH%c", posX, posY+52, rail);
				printf("\033[%d;%dH%s\n", posX+1, posY, montrain.custom[1]);
				printf("\033[%d;%dH%c", posX+1, posY+52, rail);
				col--;
			}
		}
	}
	return;
}

void afficherSortTrain(TRAIN montrain, int rail, int posX, int premierwagon, int val, int i){
	if (montrain.direction == 'e'){
		printf("\033[%d;%dH%c", posX, i-1, rail);
		for(int j=0; j<premierwagon; j++){
			if(i+j>74){
				return;
			}
			else {
				printf("\033[%d;%dH%c\n", posX, i+j, montrain.custom[val][j]);
			}
		}
	}
	else{
		for(int j=53; j>premierwagon; j--){
			if(j==premierwagon){
				return;
			}
			else {
				printf("\033[%d;%dH%c\n", posX, j, rail);
			}
		}
	}
}

void entreeTrain(TRAIN montrain, int posX, int time){
	char rail = '-';
	if (montrain.direction == 'e'){
		int dernierwagon=montrain.longueur-1;
		int i=0;
		while(dernierwagon>=0){
			time=(time+2)%montrain.vitesse;
			if(time==1000){
				for(int j=0; j<i; j++){
					printf("\033[%d;%dH%c\n", posX, i, montrain.custom[0][51-j]);
					printf("\033[%d;%dH%c\n", posX, i+1, 'd');
					printf("\033[%d;%dH%c\n", posX+1, i, montrain.custom[1][dernierwagon]);
				}
				dernierwagon--;
				i++;
			}
		}
	}
	else{
		int premierwagon=75;
		int i=75;
		while(i>=23){
			time=(time+2)%montrain.vitesse;
			if(time==1000){
				afficherSortTrain(montrain, rail, posX, premierwagon, 0, i);
				afficherSortTrain(montrain, rail, posX+1, premierwagon, 1, i);
				premierwagon--;
				i--;
			}
		}
	}
}//entre vite puis réduit sa vitesse

void sortieTrain(TRAIN montrain, int rail, int posX, int time, int vitesse, char direction){
	if(direction == 'e'){
		int premierwagon=52;
		int i=24;
		while(i<=75){
			time=(time+2)%vitesse;
			if(time==1000){
				printf("\033[%d;%dH%c", posX, i-1, rail);
				afficherSortTrain(montrain, rail, posX, premierwagon, 0, i);
				printf("\033[%d;%dH%c", posX+1, i-1, rail);
				afficherSortTrain(montrain, rail, posX+1, premierwagon, 1, i);
				premierwagon--;
				i++;
			}
		}
	}
	else {
		int premierwagon=52;
		while(premierwagon>=0){
			time=(time+2)%vitesse;
			if(time==1000){
				afficherSortTrain(montrain, rail, posX, premierwagon, 0, premierwagon);
				afficherSortTrain(montrain, rail, posX+1, premierwagon, 1, premierwagon);
				premierwagon--;
			}
		}
	}
}

// int main(){
// 	//int tempsAttente=3;
// 	FILE * train = fopen("txt/train_test.txt", "r");
// 	TRAIN montrain = init_train(train);
// 	mvtTrain(montrain, 1);
// 	//afficher_train (train, T);
// 	return 0;
// }
