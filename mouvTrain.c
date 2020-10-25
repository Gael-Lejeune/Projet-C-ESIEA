void deplacementTrain(TRAIN montrain, int rail, int col, int vitesse, int time, int posX, int posY, int posArret, char direction){
	if (direction == 'e'){
		while(col<posArret){
			time=(time+1)%vitesse;
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
	else { //direction = 'o'
		while(col>posArret){
			time=(time+1)%vitesse;
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

void afficherSortTrain(TRAIN montrain, int rail, int posX, int premierwagon, int val, int i, char direction);

void entreeTrain(TRAIN montrain, int posX, int posY, int time, int vitesse, char direction){
	int rail = '_';
	if (direction == 'e'){
		int dernierwagon=52;
		int i=0;
		while(dernierwagon>=0){
			time=(time+2)%vitesse;
			if(time==1000){
				printf("\033[%d;%dH%c\n", posX, i, montrain.custom[0][dernierwagon]);
				printf("\033[%d;%dH%c\n", posX+1, i, montrain.custom[1][dernierwagon]);
				dernierwagon--;
				i++;
			}
		}
	}
	else{
		int premierwagon=75;
		int i=75;
		while(i>=23){
			time=(time+2)%vitesse;
			if(time==1000){
				afficherSortTrain(montrain, rail, posX, premierwagon, 0, i, 'e');
				afficherSortTrain(montrain, rail, posX+1, premierwagon, 1, i, 'e');
				premierwagon--;
				i--;
			}
		}
	}
}//entre vite puis réduit sa vitesse

void afficherSortTrain(TRAIN montrain, int rail, int posX, int premierwagon, int val, int i, char direction){
	if (direction == 'e'){
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

void sortieTrain(TRAIN montrain, int rail, int posX, int time, int vitesse, char direction){
	if(direction == 'e'){
		int premierwagon=52;
		int i=24;
		while(i<=75){
			time=(time+2)%vitesse;
			if(time==1000){
				printf("\033[%d;%dH%c", posX, i-1, rail);
				afficherSortTrain(montrain, rail, posX, premierwagon, 0, i, direction);
				printf("\033[%d;%dH%c", posX+1, i-1, rail);
				afficherSortTrain(montrain, rail, posX+1, premierwagon, 1, i, direction);
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
				afficherSortTrain(montrain, rail, posX, premierwagon, 0, premierwagon, direction);
				afficherSortTrain(montrain, rail, posX+1, premierwagon, 1, premierwagon, direction);
				premierwagon--;
			}
		}
	}
}

int main(){
	system("clear");
	FILE * train = fopen("txt/train_test.txt", "r");
	TRAIN montrain = init_train(train);
	int time=0, col=0, posX=7, posY=0, vitesse=10000000;
	FILE * gare = fopen("txt/gare_test.txt", "r");
	GARE magare = init_gare(gare);
          afficher_gare(gare, magare);
          int rail = '_';
          entreeTrain(montrain, posX, posY, time, vitesse, 'e');
	deplacementTrain(montrain, rail, col, vitesse, time, posX, posY, 13, 'e'); //un "_" vient dans cette fonction. ;-;
	//13 : posArret : position pour laquelle le train est correctement à quai
	//Second train :
	entreeTrain(montrain, posX+3, posY, time, vitesse, 'o');
	deplacementTrain(montrain, rail, 22, vitesse, time, posX+3, posY, 12, 'o');
	
	//sleep(2); //temps d'arrêt du train
	//Premier train :
	deplacementTrain(montrain, rail, 12, vitesse, time, posX, posY, 24, 'e');
	//12 : col : position de l'arrière du train à l'arrêt ; 24 : posArret : position pour laquelle le train est complètement dans la gare et s'apprête à en sortir 
	//(pos 75 = lim droite)
	sortieTrain(montrain, rail, posX, time, vitesse, 'e');
	//sleep(1);
	deplacementTrain(montrain, rail, 12, vitesse, time, posX+3, posY, 0, 'o');
	sortieTrain(montrain, rail, posX+3, time, vitesse, 'o');
	printf("\n\n\n\n\n\n\n\n\n\n");
	return 0;
}
