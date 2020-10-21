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



