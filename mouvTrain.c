void deplacementTrain(TRAIN montrain, int col, int vitesse, int time, int posX, int posY, int posArret){
	while(col<posArret){
		time=(time+1)%vitesse;
		if(time==1000){
			posY=col;
			printf("\033[%dH﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘", posX);
			printf("\033[%d;%dH%s\n", posX, posY, montrain.custom[0]);
			printf("\033[%dH﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘﹘", posX+1);
			printf("\033[%d;%dH%s\n", posX+1, posY, montrain.custom[1]);
			col++;
			}
	}
	return;
}

void entreeTrain(TRAIN montrain, int posX, int posY, int time, int vitesse){
	int dernierwagon=52;
	int i=0;
	while(dernierwagon>=0){
		time=(time+1)%vitesse;
		if(time==1000){
			printf("\033[%d;%dH%c\n", posX, i, montrain.custom[1][dernierwagon]);
			printf("\033[%d;%dH%c\n", posX+1, i, montrain.custom[1][dernierwagon]);
			dernierwagon--;
			i++;
		}
	}
}

/*	int time=0, col=1, posX=7, posY=0, vitesse=10000000;
	entreeTrain(montrain, posX, posY, time, vitesse);
	deplacementTrain(montrain, col, vitesse, time, posX, posY, 13);
	//13 : posArret : position pour laquelle le train est correctement à quai
	sleep(2); //temp d'arrêt du train
	deplacementTrain(montrain, 12, vitesse, time, posX, posY, 24);
	//12 : col : position de l'arrière du train à l'arrêt ; 24 : posArret : position pour laquelle le train est complètement dans la gare et s'apprête à en sortir */
