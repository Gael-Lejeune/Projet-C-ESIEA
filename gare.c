#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "gare.h"

GARE init_gare(char * fichier){
    //Initialisation de la gare via fichier txt
    FILE * gare = fopen(fichier, "r"); //Fichier de l'affichage de la gare
    GARE magare;
    magare.custom = (char **)malloc(LARGEUR*sizeof(char *));
    for(int i =0; i<LARGEUR; i++) {
        magare.custom[i] = (char *)malloc(LONGUEUR*sizeof(char));
    } //Allocation de l'espace mémoire pour la gare.
    char c;
    for(int i=0; i<LARGEUR; i++){
        for(int j=0; j<LONGUEUR; j++){
            c = fgetc(gare);
            magare.custom[i][j] = c;
        }
        fgetc(gare);
    } //Remplissage de la gare en lisant le fichier txt correspondant.
    fclose(gare);
    return magare;
} //init_gare()

void afficher_gare (GARE magare){ //affichage de la gare
    system("clear"); //clear du terminal
    for(int i=0; i<LARGEUR; i++){ //Pour la largeur de la gare
        for(int j=0; j<LONGUEUR; j++){ //Pour la longueur de la gare
            switch (magare.custom[i][j]) { //On affiche la gare en ascii etendu à partir du fichier écrit en ascii simple
                case '=': //mur
                printf("%s%s%s", WALLCOLOR, "═", DEFAULT_COLOR);
                break;
                case 'a': //mur
                printf("%s%s%s", WALLCOLOR, "╔", DEFAULT_COLOR);
                break;
                case 'c': //mur
                printf("%s%s%s", WALLCOLOR, "╝", DEFAULT_COLOR);
                break;
                case 'w': //mur
                printf("%s%s%s", WALLCOLOR, "╚", DEFAULT_COLOR);
                break;
                case 'e': //mur
                printf("%s%s%s", WALLCOLOR, "╗", DEFAULT_COLOR);
                break;
                case '_': //bord du quai
                printf("%s%s%s", GREY, "■", DEFAULT_COLOR);
                break;
                case '|': //mur
                printf("%s%s%s", WALLCOLOR, "║", DEFAULT_COLOR);
                break;
                case 'l':
                printf("%s%s%s", WALLCOLOR, "═", DEFAULT_COLOR);
                break;
                case '-': //rails
                printf("%s%s%s", RAILCOLOR, "─", DEFAULT_COLOR);
                break;
                case '*': //voyageur
                printf("%s%s%s", PLAYERCOLOR, "*", DEFAULT_COLOR);
                break;
                case '#': //bord du compteur
                printf("%s%s%s", RAILCOLOR, "│", DEFAULT_COLOR);
                break;
                case 'u': //bord du compteur
                printf("%s%s%s", RAILCOLOR, "┘", DEFAULT_COLOR);
                break;
                case 'i': //bord du compteur
                printf("%s%s%s", RAILCOLOR, "└", DEFAULT_COLOR);
                break;
                case 'j': //bord du compteur
                printf("%s%s%s", RAILCOLOR, "┐", DEFAULT_COLOR);
                break;
                case 'k': //bord du compteur
                printf("%s%s%s", RAILCOLOR, "┌", DEFAULT_COLOR);
                break;
                case '+': //bord du compteur
                printf("%s%s%s", RAILCOLOR, "─", DEFAULT_COLOR);
                break;
                case 'b': //bord du compteur
                printf("%s%s%s", RAILCOLOR, "|", DEFAULT_COLOR);
                break;
                default: //Si le caractère n'est pas reconnu
                printf("%c", magare.custom[i][j]);
                break;
            }
        }
        printf("\n");
    }
}
