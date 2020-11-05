#include <stdio.h>
#include <stdlib.h>

typedef struct TRAIN train;
struct TRAIN{
    char direction ;    /*N => Nord, S => Sud, E => EST, O => OUEST*/
    int posx;           /*Position courante x de la tête du train*/
    int posy;           /*Position courante y de la tête du train*/
    int vitesse;        /*Vitesse du train*/
    char ** custom;   /*Contient le train customisé, il faut choisir la bonne taille de votre tableau*/
    char etat;          /*État du train => dehors, entrant, stationné, sortant, sorti*/
    int portes;        /*Portes ouvertes ou fermées*/
    int longueur;       /*Longueur du train*/
};

void charger_train(){
    FILE *fichier = fopen("txt/train_test.txt", "r+");
    // Récupère le fichier et l'ouvre en mode lecture et écriture.
    // Le fichier doit exister.

    if (fichier == NULL){
        printf("Le fichier train_test.txt n'a pas pu être ouvert\n");
        // return EXIT_FAILURE;
    }
    // Teste l'existence du fichier.

    char c;
    while(1) {
        c = fgetc(fichier);
        if( feof(fichier) ) {
            break ;
        }
        printf("%c", c);
    }
    // récupère les éléments du fichier, un par un.

    fclose(fichier);
    // Ferme le fichier.
}

char ** ALLOCATION_MAT_DYN(int NB_L, int NB_C) {
	char ** tab = (char **)malloc(NB_L*sizeof(char *));
	for(int i =0; i<NB_L;i++) {
		tab[i] = (char *)malloc(NB_C*sizeof(char));
	}
	return tab;
}

struct TRAIN init_train(FILE * fichier){
	struct TRAIN T;
	T.custom = (char **)malloc(2*sizeof(char *));
	for(int i =0; i<2;i++) {
		T.custom[i] = (char *)malloc(52*sizeof(char));
	}
	char c;
	for(int i=0; i<2; i++){
		for(int j=0; j<52; j++){
			c = fgetc(fichier);
			T.custom[i][j] = c;
		}
		fgetc(fichier);
	}
	T.direction = 'e';
	T.posx=7;
	T.posy=0;
	T.vitesse= 10000000;
	T.etat='d'; //dehors
	T.portes=0; //fermées
	T.longueur=52;
	return T;
}

/*void mvtTrain(struct TRAIN T, int tempsAttente){
	switch (T.etat){
		case 'd':
			if (tempsAttente<=2){
				T.etat='e';
			}
			tempsAttente--;
			break;
		case 'e':
			mvtTrainEnGare(T);
	}
}

int main(){
	//int tempsAttente=3;
	FILE * train = fopen("txt/train_test.txt", "r");
	struct TRAIN T = init_train(train);
	//mvtTrain(T);
	return 0;
}*/
