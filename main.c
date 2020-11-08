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

#define PURPLE          "\033[1;35m"
#define CYAN            "\033[1;36m"
#define TRAINCOLOR      PURPLE
#define DOORCOLOR      	CYAN
#define DEFAULT_COLOR   "\033[0;m"


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
        if(feof(fichier)) {
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
    system("clear");
    // system("setterm -cursor on");
    // system("setterm -cursor off");
    afficher_fichier("txt/menu_projet.txt");
    char c;
    char option = '1';
    printf("\033[%d;%dH%s≫%s", 14, 10, "\033[0;m", "\033[0;m");
    printf("\033[%d;%dH%s≫%s", 13, 10, "\033[1;5m", "\033[0;m");
    printf("\033[%d;%dH", 25, 0);
    while(1){
        c = key_pressed();
        if (c == 'z') {
            option = '1';
            printf("\033[%d;%dH%s≫%s", 14, 10, "\033[0;m", "\033[0;m");
            printf("\033[%d;%dH%s≫%s", 13, 10, "\033[1;5m", "\033[0;m");
            printf("\033[%d;%dH", 25, 0);
        }
        else if (c == 's') {
            option = '2';
            printf("\033[%d;%dH%s≫%s", 13, 10, "\033[0;m", "\033[0;m");
            printf("\033[%d;%dH%s≫%s", 14, 10, "\033[1;5m", "\033[0;m");
            printf("\033[%d;%dH", 25, 0);
        }
        else if (c == 'o'){
            break;
        }
        else {
            printf("\033[%d;%dH ", 25, 0);
        }
    }
    // while (option == 0x00) {
    //     option = key_pressed();
    // }

    printf("Vous avez choisi l'option %c\n", option);

    FILE * gare = fopen("txt/gare_test.txt", "r");
    GARE magare = init_gare(gare);

    if (option == '1') {
        afficher_gare(magare);
        FILE * train = fopen("txt/train_test2.txt", "r");
        TRAIN montrain = init_train(train, 'o');

        int time = 0;
        int posX, posY;
        char* rail = "─";
        int lDoor;
        int tempsAQuai = montrain.tempsAQuai;

        while(1){
            time=(time+1)%montrain.vitesse;
            // printf("\033[%d;%dHTime : %d", 30, 20, time);
            if(time == 500){
                if (montrain.direction == 'e') {
                    lDoor = 11;
                    // printf("test\n");
                    switch (montrain.etat){
                        case 'd': //dehors
                        if (montrain.tempsAttente == 0){
                            montrain.etat='e';
                        }
                        else{
                            montrain.tempsAttente--;
                        }
                        break;

                        case 'e':
                        for (int i = montrain.posx; i >= 0; i--) {
                            afficherCarTrain(montrain.custom[0][montrain.longueur - i], lDoor - 1, montrain.posx-i);
                            afficherCarTrain(montrain.custom[1][montrain.longueur - i], lDoor, montrain.posx-i);
                        }
                        if (montrain.posx > 52) {
                            printf("\033[%d;%dH%s\n", lDoor - 1, montrain.posx - montrain.longueur - 1, "─");
                            printf("\033[%d;%dH%s\n", lDoor, montrain.posx - montrain.longueur - 1, "─");
                        }
                        montrain.posx++;
                        if(montrain.posx == 58){
                            montrain.etat = 's';
                        }
                        break;

                        case 's' :
                        if (tempsAQuai == montrain.tempsAQuai) {
                            for (int i = montrain.posx; i >= 0; i--) {
                                if (montrain.custom[1][montrain.longueur - i] == 'f') {
                                    printf("\033[%d;%dH%c\n", lDoor, montrain.posx-i-1, ' ');
                                }
                            }
                        }
                        else if (tempsAQuai == 0) {
                            for (int i = montrain.posx; i >= 0; i--) {
                                if (montrain.custom[1][montrain.longueur - i] == 'f') {
                                    printf("\033[%d;%dH%s%s%s\n", lDoor, montrain.posx-i-1, DOORCOLOR, "-", DEFAULT_COLOR);
                                }
                            }
                            montrain.etat = 'p';
                        }
                        tempsAQuai--;
                        break;

                        case 'p' :
                        for (int i = montrain.longueur; i >= 0; i--) {
                            if (montrain.posx - i <= 62) {
                                afficherCarTrain(montrain.custom[0][montrain.longueur - i], lDoor - 1, montrain.posx-i);
                                afficherCarTrain(montrain.custom[1][montrain.longueur - i], lDoor, montrain.posx-i);
                            }
                        }
                        printf("\033[%d;%dH%s\n", lDoor - 1, montrain.posx - montrain.longueur - 1, "─");
                        printf("\033[%d;%dH%s\n", lDoor, montrain.posx - montrain.longueur - 1, "─");
                        montrain.posx++;
                        if(montrain.posx == montrain.longueur + 1 + 62 + 1){
                            montrain.etat = 'r';
                        }

                        break;

                        // printf("test\n");
                    }
                }

                else{
                    lDoor = 7;
                    switch (montrain.etat){
                        case 'd': //dehors
                        if (montrain.tempsAttente == 0){
                            montrain.etat='e';
                        }
                        else{
                            montrain.tempsAttente--;
                        }
                        break;

                        case 'e':
                        for (int i = montrain.posx; i <= 62; i++) {
                            afficherCarTrain(montrain.custom[0][i - montrain.posx], lDoor, i);
                            afficherCarTrain(montrain.custom[1][i - montrain.posx], lDoor + 1, i);
                        }
                        if (montrain.posx < 11) {
                            printf("\033[%d;%dH%s\n", lDoor, montrain.posx + montrain.longueur, "─");
                            printf("\033[%d;%dH%s\n", lDoor + 1, montrain.posx + montrain.longueur, "─");
                        }
                        montrain.posx--;
                        if(montrain.posx == 6){
                            montrain.etat = 's';
                        }
                        break;
                        case 's' :
                        if (tempsAQuai == montrain.tempsAQuai) {
                            for (int i = montrain.posx; i <= montrain.posx + montrain.longueur; i++) {
                                if (montrain.custom[0][montrain.longueur - i] == 'd') {
                                    printf("\033[%d;%dH%c\n", lDoor, montrain.posx+i, ' ');
                                }
                            }
                        }
                        else if (tempsAQuai == 0) {
                            for (int i = montrain.posx; i <= montrain.posx + montrain.longueur; i++) {
                                if (montrain.custom[0][montrain.longueur - i] == 'd') {
                                    printf("\033[%d;%dH%s%s%s\n", lDoor, montrain.posx+i, DOORCOLOR, "-", DEFAULT_COLOR);
                                }
                            }
                            montrain.etat = 'p';
                        }
                        tempsAQuai--;
                        break;

                        case 'p' :
                        for (int i = montrain.longueur; i >= 0; i--) {
                            if (montrain.posx + i >= 0) {
                                afficherCarTrain(montrain.custom[0][i], lDoor, montrain.posx+i);
                                afficherCarTrain(montrain.custom[1][i], lDoor + 1, montrain.posx+i);
                            }
                        }
                        printf("\033[%d;%dH%s\n", lDoor, montrain.posx + montrain.longueur, "─");
                        printf("\033[%d;%dH%s\n", lDoor + 1, montrain.posx + montrain.longueur, "─");
                        montrain.posx--;
                        if(montrain.posx == -montrain.longueur){
                            montrain.etat = 'r';
                        }

                        break;

                    }
                }

                printf("\033[%d;%dHCoordonees :           ", 30, 20);
                printf("\033[%d;%dHCoordonees : %c %d", 30, 20, montrain.etat, montrain.posx);
            }

        }
        fclose(gare);
    }
    else if (option == '2'){
        VOYAGEUR monvoyageur = init_voyageur(0, 14, '*', magare);
        char movPlayer = 0;

        afficher_gare(magare);
        printf("\033[%d;%dHCoordonees : %d %d", 30, 20, monvoyageur->posX, monvoyageur->posY);
        while(1){
            movPlayer = key_pressed();
            if (movPlayer != 0) {
                mvtVoy(monvoyageur, magare, movPlayer);
                // printf("Coordonees : %d %d\n", monvoyageur->posX, monvoyageur->posY );
                printf("\033[%d;%dHCoordonees : %d %d", 30, 20, monvoyageur->posX, monvoyageur->posY);
            }

        }
        fclose(gare);
    }
    return 0;
}
