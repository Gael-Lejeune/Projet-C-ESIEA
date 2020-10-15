#include <stdio.h>
#include <stdlib.h>


//Initialise un tableau à deux dimensions contenant la lettre demandée//
void initBiDChar(int nb_l, int nb_c, char array[nb_l][nb_c], char lettre){
        int i;
        for(i=0; i<nb_l; i++){
                int j;
                for(j=0; j<nb_c; j++){
                        array[i][j]=lettre;
                        //printf("%c ", array[i][j]);
                }
                //printf("\n");
        }
}

//Initialise un tableau à deux dimensions contenant une matrice de postions//
void initBiDChar2(int nb_l, int nb_c, char array[nb_l][nb_c], char lettre1, char lettre2){
        int i;
        for(i=0; i<nb_l; i++){
                int j; int c=0;
                for(j=0; j<nb_c; j++){
			if (c==3){
				array[i][j]=lettre2;
				//printf("%c", array[i][j]);
				c=0;
			}
			else {
				array[i][j]=lettre1;
                       		//printf("%c ", array[i][j]);
				c++;
			}
                }
                //printf("\n");
        }
}


void showBiD(int nb_lignes, int nb_colonnes, char array[nb_lignes][nb_colonnes]){
	int i;
	for(i=0; i<nb_lignes; i++){
		int j;
		for(j=0; j<nb_colonnes; j++){
			printf("%c ", array[i][j]);
		}
		printf("\n");
	}
}

// Vérifie si la position voulue par l'utilisateur est libre.

int posLibre(int Vl, int Vc, int nb_l, int nb_c, char mP[nb_l][nb_c], int l, int c){
        int L = Vl + l;
        int C = Vc + c;
        char libre = mP[L][C];
        switch (libre){
                case 's' :
			mP[Vl][Vc]='l';
                        mP[L][C]='u';
			return 0;
		case 'l' :
                        mP[Vl][Vc]='l';
                        mP[L][C]='u';
                        return 0;
                case 'e' :
                        mP[Vl][Vc]='l';
                        if (L==0){L=nb_l-1;}
                        if (L==nb_l){L=1;};
                        mP[L][C]='u';
                        return 0;
                case 'a' :
                        mP[Vl][Vc]='l';
                        return 1;
                case 'd' :
                        return 2;
		case 'v' :
			return 2;
		default :
			return 2;
        }
}


///////////// Fonction mouvement voyageur /////////////

void mvtVoy(int posVoyL, int posVoyC, int nb_l, int nb_c, char matGare[nb_l][nb_c], char matPos[nb_l][nb_c]){
	printf("Entrer la direction vers laquelle vous souhaitez vous diriger : \n");
	char mvt;
	scanf("%c", &mvt);
	int directive, l, c;
	switch (mvt){
		case 'd' :
			l=0; c=1;
			break;
		case 'q' :
			l=0; c=-1;
                        break;
                case 's' :
			l=1; c=0;
                        break;
                case 'z' :
			l=-1; c=0;
                        break;
		default :
			l=c=0;
			break;
	}
	directive = posLibre(posVoyL, posVoyC, nb_l, nb_c, matPos, l, c);
	switch (directive){
		case 0 :
			matGare[posVoyL][posVoyC]=' ';
			matGare[posVoyL+l][posVoyC+c]='u';
			break;
		case 1 :
			matGare[posVoyL][posVoyC]=' ';
			system("clear");
			showBiD(nb_l, nb_c, matGare);
			printf("Bravo, vous avez pris le métro !\n");
			return;
		case 2 :
			l=c=0;
			break;	
	}
	//monterTrain();
	//showBiD(nb_l, nb_c, matPos);
	system("clear");
	showBiD(nb_l, nb_c, matGare);
	mvtVoy((posVoyL+l), (posVoyC+c), nb_l, nb_c, matGare, matPos);
}


int main(){
	int nb_l = 4;
	int nb_c = 4;
	char matGare[nb_l][nb_c], matPos[nb_l][nb_c];
	initBiDChar(nb_l, nb_c, matGare, 'c');
	initBiDChar2(nb_l, nb_c, matPos, 'l', 'a');
	mvtVoy(0,1, nb_l, nb_c, matGare, matPos);
	//printf("\n\n\n");
	//showBiD(nb_l, nb_c, matPos);
        //showBiD(nb_l, nb_c, matGare);
	return 0;
}
