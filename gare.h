#include <stdio.h>
#include <stdlib.h>

#define LONGUEUR 102
#define LARGEUR 17

#define BLACK    "\033[1;30m"
#define RED      "\033[1;31m"
#define GREEN    "\033[1;32m"
#define YELLOW   "\033[1;33m"
#define BLUE     "\033[1;34m"
#define PURPLE   "\033[1;35m"
#define CYAN     "\033[1;36m"
#define GREY     "\033[1;37m"
#define DEFAULT_COLOR "\033[0;m"

typedef struct gare GARE;
struct gare{
    char ** custom;   /*Contient le train customisé, il faut choisirla bonne taille de votre tableau*/
};

char ** ALLOCATION_MAT_DYN(int NB_L, int NB_C) {
	char ** tab = (char **)malloc(NB_L*sizeof(char *));
	for(int i =0; i<NB_L;i++) {
		tab[i] = (char *)malloc(NB_C*sizeof(char));
	}
	return tab;
}

GARE init_gare(FILE * fichier){
    GARE magare;

    magare.custom = ALLOCATION_MAT_DYN(LARGEUR,LONGUEUR);
    char c;
    for(int i=0; i<LARGEUR; i++){
        for(int j=0; j<LONGUEUR; j++){
            c = fgetc(fichier);
            if (c == -30) {
                magare.custom[i][j] = 'm';
            }
            else {
                c = fgetc(fichier);
            }
        }
    }
    // printf("\n");
    return magare;
}

void afficher_gare (FILE * fichier,GARE magare){
    fseek(fichier, 0, 0);
    for(int i=0; i<LARGEUR; i++){
        for(int j=0; j<LONGUEUR; j++){
            if (magare.custom[i][j] == 'm') {
                printf("%s%c%s", RED,fgetc(fichier), DEFAULT_COLOR);
                // printf("%c", magare.custom[i][j]);
                // printf("%c",fgetc(fichier));
            }
            else {
                printf("%c",fgetc(fichier));
                // printf("%c",201);
            }
            // printf("%c", magare.custom[i][j]);
            // printf("%s", DEFAULT_COLOR);
        }
    }
    printf("\n");
}
