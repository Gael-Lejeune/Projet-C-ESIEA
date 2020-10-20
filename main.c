#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
// #include <ncurses.h>
#include "train.h"
#include "gare.h"
#include "voyageur.h"

char key_pressed(){
    struct termios oldterm, newterm;
    int oldfd;
    char c, result = 0;
    tcgetattr (STDIN_FILENO, &oldterm);
    newterm = oldterm;
    newterm.c_lflag &= ~(ICANON | ECHO);
    tcsetattr (STDIN_FILENO, TCSANOW, &newterm);
    oldfd = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl (STDIN_FILENO, F_SETFL, oldfd | O_NONBLOCK);
    c = getchar();
    tcsetattr (STDIN_FILENO, TCSANOW, &oldterm);
    fcntl (STDIN_FILENO, F_SETFL, oldfd);
    if (c != EOF) {
        ungetc(c, stdin);
        result = getchar();
    }
    return result;
}

void afficher_fichier(char * nomfic){
    FILE *fichier = fopen(nomfic, "r+");
    // Récupère le fichier et l'ouvre en mode lecture et écriture.
    // Le fichier doit exister.

    if (fichier == NULL){
        printf("Le fichier menu_projet.txt n'a pas pu être ouvert\n");
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

int main() {
    // char fichier[20] = "txt/menu.txt";
    afficher_fichier("txt/menu_projet.txt");
    char option;
    while (option == 0x00) {
        option = key_pressed();
    }

    // printf("option = %c\n", option);
    // char option;
    // int n;
    // n = scanf("%d",&option);
    // if (n != 1){
    //     printf("Input error\n");
    //     exit(-1);
    // }
    printf("Vous avez choisi l'option %c\n", option);

    if (option == '1') {
        FILE * gare = fopen("txt/gare_test.txt", "r");
        GARE magare = init_gare(gare);
        VOYAGEUR monvoyageur = init_voyageur(0, 14, '*', magare);
        char mov = 0;
        // printf("coucou\n");
        // int i = 0;
        afficher_gare(gare, magare);
        // afficher_voyageur(monvoyageur);
        while(1){
            mov = key_pressed();
            if (mov != 0) {
                mvtVoy(monvoyageur, magare, mov);
            }
            // printf("%d %d\n", monvoyageur.posX, monvoyageur.posY);
            // usleep(100000);
            // system("clear");
            // mov = 0;
            // printf("%d\n", i);
        }
        fclose(gare);
    }
    return 0;
}
