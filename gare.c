#include <stdio.h>
#include <stdlib.h>

#define LONGUEUR 64
#define LARGEUR 16

#define BLACK    "\033[1;30m"
#define RED      "\033[1;31m"
#define GREEN    "\033[1;32m"
#define YELLOW   "\033[1;33m"
#define BLUE     "\033[1;34m"
#define PURPLE   "\033[1;35m"
#define CYAN     "\033[1;36m"
#define GREY     "\033[1;37m"
#define DEFAULT_COLOR "\033[0;m"
#define WALLCOLOR RED
#define RAILCOLOR GREY
#define PLAYERCOLOR CYAN

typedef struct gare GARE;
struct gare{
    char ** custom;   /*Contient le train customisé, il faut choisirla bonne taille de votre tableau*/
};

GARE init_gare(FILE * fichier){
    GARE magare;
    // magare.custom = ALLOCATION_MAT_DYN(LARGEUR,LONGUEUR);
    magare.custom = (char **)malloc(LARGEUR*sizeof(char *));
    for(int i =0; i<LARGEUR;i++) {
        magare.custom[i] = (char *)malloc(LONGUEUR*sizeof(char));
    }
    char c;
    for(int i=0; i<LARGEUR; i++){
        for(int j=0; j<LONGUEUR; j++){
            c = fgetc(fichier);
            magare.custom[i][j] = c;
        }
        fgetc(fichier);
    }

    // printf("\n");
    return magare;
}

void afficher_gare (FILE * fichier,GARE magare){
    system("clear");
    fseek(fichier, 0, 0);
    for(int i=0; i<LARGEUR; i++){
        for(int j=0; j<LONGUEUR; j++){
            switch (magare.custom[i][j]) {
                case '=':
                // printf("%s%c%s", WALLCOLOR, 201, DEFAULT_COLOR);
                printf("%s%s%s", WALLCOLOR, "═", DEFAULT_COLOR);
                break;
                case 'a':
                printf("%s%s%s", WALLCOLOR, "╔", DEFAULT_COLOR);
                break;
                case 'c':
                printf("%s%s%s", WALLCOLOR, "╝", DEFAULT_COLOR);
                break;
                case 'w':
                printf("%s%s%s", WALLCOLOR, "╚", DEFAULT_COLOR);
                break;
                case 'e':
                printf("%s%s%s", WALLCOLOR, "╗", DEFAULT_COLOR);
                break;
                case '_':
                printf("%s%s%s", WALLCOLOR, "■", DEFAULT_COLOR);
                break;
                case '|':
                printf("%s%s%s", WALLCOLOR, "║", DEFAULT_COLOR);
                break;
                case 'l':
                printf("%s%s%s", WALLCOLOR, "═", DEFAULT_COLOR);
                break;
                case '-':
                printf("%s%s%s", RAILCOLOR, "─", DEFAULT_COLOR);
                break;
                case '*':
                printf("%s%s%s", PLAYERCOLOR, "*", DEFAULT_COLOR);
                break;
                default:
                printf("%c", magare.custom[i][j]);
                break;
            }
        }
        // fgetc(fichier);
    }
    printf("\n");
}
