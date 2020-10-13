#include <stdio.h>
#include <stdlib.h>

#define LONGUEUR 120
#define LARGEUR 17

typedef struct gare GARE;
struct gare{
    char ** custom;   /*Contient le train customisé, il faut choisirla bonne taille de votre tableau*/
};

char ** ALLOCATION_GARE_DYN(int NB_L, int NB_C) {
	char ** tab = (char **)malloc(NB_L*sizeof(char *));
	for(int i =0; i<NB_L;i++) {
		tab[i] = (char *)malloc(NB_C*sizeof(char));
	}
	return tab;
}

GARE charger_gare(){
    // FILE *fichier = fopen("txt/gare_test.txt", "r+");
    GARE magare;

    magare.custom = ALLOCATION_GARE_DYN(LARGEUR,LONGUEUR);

    int i, j;
    FILE * fichier = fopen("txt/gare_test.txt", "r");
    for(i=0; i<LARGEUR; i++){
        for(j=0; j<LONGUEUR; j++){
            fscanf(fichier,"%c",  &magare.custom[i][j]);
        }
    }
    fclose(fichier);

    for(int i=0; i<LARGEUR; i++){
        for(int j=0; j<LONGUEUR; j++){
            printf("%c", magare.custom[i][j]);
        }
    }
    printf("\n");

    return magare;
}
