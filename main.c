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
#include "listeChainee.h"

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
        LISTE maListe; init_liste(&maListe);
        ajoutD(&maListe,init_train(train, 'o'));
        ajoutD(&maListe,init_train(train, 'e'));
        printf("test\n");
        View(maListe);
        // TRAIN monElementTrain->train = init_train(train, 'o');

        int time = 0;
        //int posX, posY;
        //char* rail = "─";
        int lDoor;
        ELEMENT* monElementTrain = maListe.premier;
        int tempsAQuai = monElementTrain->train.tempsAQuai;
        int tempsAQuaiP = maListe.premier->train.tempsAQuai;
        int tempsAQuaiD = maListe.dernier->train.tempsAQuai;

        while(1){
            // printf("\033[%d;%dHTime : %d", 30, 20, time);
            if(time == 500){
                if (monElementTrain->train.direction == 'e') {
                    lDoor = 11;
                    // printf("test\n");
                    switch (monElementTrain->train.etat){
                        case 'd': //dehors
                        if (monElementTrain->train.tempsAttente == 0){
                            monElementTrain->train.etat='e';
                        }
                        else{
                            monElementTrain->train.tempsAttente--;
                        }
                        break;

                        case 'e':
                        for (int i = monElementTrain->train.posx; i >= 0; i--) {
                            afficherCarTrain(monElementTrain->train.custom[0][monElementTrain->train.longueur - i], lDoor - 1, monElementTrain->train.posx-i);
                            afficherCarTrain(monElementTrain->train.custom[1][monElementTrain->train.longueur - i], lDoor, monElementTrain->train.posx-i);
                        }
                        if (monElementTrain->train.posx > 52) {
                            printf("\033[%d;%dH%s\n", lDoor - 1, monElementTrain->train.posx - monElementTrain->train.longueur - 1, "─");
                            printf("\033[%d;%dH%s\n", lDoor, monElementTrain->train.posx - monElementTrain->train.longueur - 1, "─");
                        }
                        monElementTrain->train.posx++;
                        if(monElementTrain->train.posx == 58){
                            monElementTrain->train.etat = 's';
                        }
                        break;

                        case 's' :
                        if (tempsAQuai == monElementTrain->train.tempsAQuai) {
                            for (int i = monElementTrain->train.posx; i >= 0; i--) {
                                if (monElementTrain->train.custom[1][monElementTrain->train.longueur - i] == 'f') {
                                    printf("\033[%d;%dH%c\n", lDoor, monElementTrain->train.posx-i-1, ' ');
                                }
                            }
                        }
                        else if (tempsAQuai == 0) {
                            for (int i = monElementTrain->train.posx; i >= 0; i--) {
                                if (monElementTrain->train.custom[1][monElementTrain->train.longueur - i] == 'f') {
                                    printf("\033[%d;%dH%s%s%s\n", lDoor, monElementTrain->train.posx-i-1, DOORCOLOR, "-", DEFAULT_COLOR);
                                }
                            }
                            monElementTrain->train.etat = 'p';
                        }
                        tempsAQuai--;
                        break;

                        case 'p' :
                        for (int i = monElementTrain->train.longueur; i >= 0; i--) {
                            if (monElementTrain->train.posx - i <= 62) {
                                afficherCarTrain(monElementTrain->train.custom[0][monElementTrain->train.longueur - i], lDoor - 1, monElementTrain->train.posx-i);
                                afficherCarTrain(monElementTrain->train.custom[1][monElementTrain->train.longueur - i], lDoor, monElementTrain->train.posx-i);
                            }
                        }
                        printf("\033[%d;%dH%s\n", lDoor - 1, monElementTrain->train.posx - monElementTrain->train.longueur - 1, "─");
                        printf("\033[%d;%dH%s\n", lDoor, monElementTrain->train.posx - monElementTrain->train.longueur - 1, "─");
                        monElementTrain->train.posx++;
                        if(monElementTrain->train.posx == monElementTrain->train.longueur + 1 + 62 + 1){
                            monElementTrain->train.etat = 'r';
                        }

                        break;

                        // printf("test\n");
                    }
                }

                else{
                    lDoor = 7;
                    switch (monElementTrain->train.etat){
                        case 'd': //dehors
                        if (monElementTrain->train.tempsAttente == 0){
                            monElementTrain->train.etat='e';
                        }
                        else{
                            monElementTrain->train.tempsAttente--;
                        }
                        break;

                        case 'e':
                        for (int i = monElementTrain->train.posx; i <= 62; i++) {
                            afficherCarTrain(monElementTrain->train.custom[0][i - monElementTrain->train.posx], lDoor, i);
                            afficherCarTrain(monElementTrain->train.custom[1][i - monElementTrain->train.posx], lDoor + 1, i);
                        }
                        if (monElementTrain->train.posx < 11) {
                            printf("\033[%d;%dH%s\n", lDoor, monElementTrain->train.posx + monElementTrain->train.longueur, "─");
                            printf("\033[%d;%dH%s\n", lDoor + 1, monElementTrain->train.posx + monElementTrain->train.longueur, "─");
                        }
                        monElementTrain->train.posx--;
                        if(monElementTrain->train.posx == 6){
                            monElementTrain->train.etat = 's';
                        }
                        break;
                        case 's' :
                        if (tempsAQuai == monElementTrain->train.tempsAQuai) {
                            for (int i = monElementTrain->train.posx; i <= monElementTrain->train.posx + monElementTrain->train.longueur; i++) {
                                if (monElementTrain->train.custom[0][monElementTrain->train.longueur - i] == 'd') {
                                    printf("\033[%d;%dH%c\n", lDoor, monElementTrain->train.posx+i, ' ');
                                }
                            }
                        }
                        else if (tempsAQuai == 0) {
                            for (int i = monElementTrain->train.posx; i <= monElementTrain->train.posx + monElementTrain->train.longueur; i++) {
                                if (monElementTrain->train.custom[0][monElementTrain->train.longueur - i] == 'd') {
                                    printf("\033[%d;%dH%s%s%s\n", lDoor, monElementTrain->train.posx+i, DOORCOLOR, "-", DEFAULT_COLOR);
                                }
                            }
                            monElementTrain->train.etat = 'p';
                        }
                        tempsAQuai--;
                        break;

                        case 'p' :
                        for (int i = monElementTrain->train.longueur; i >= 0; i--) {
                            if (monElementTrain->train.posx + i >= 0) {
                                afficherCarTrain(monElementTrain->train.custom[0][i], lDoor, monElementTrain->train.posx+i);
                                afficherCarTrain(monElementTrain->train.custom[1][i], lDoor + 1, monElementTrain->train.posx+i);
                            }
                        }
                        printf("\033[%d;%dH%s\n", lDoor, monElementTrain->train.posx + monElementTrain->train.longueur, "─");
                        printf("\033[%d;%dH%s\n", lDoor + 1, monElementTrain->train.posx + monElementTrain->train.longueur, "─");
                        monElementTrain->train.posx--;
                        if(monElementTrain->train.posx == -monElementTrain->train.longueur){
                            monElementTrain->train.etat = 'r';
                        }

                        break;

                    }
                }

                printf("\033[%d;%dHCoordonees :           ", 30, 20);
                printf("\033[%d;%dHCoordonees :           ", 31, 20);
                printf("\033[%d;%dHCoordonees :           ", 32, 20);
                // printf("\033[%d;%dHCoordonees : %c %d", 30, 20, monElementTrain->train.etat, monElementTrain->train.posx);
                printf("\033[%d;%dHCoordonees : %d", 30, 20,tempsAQuai);
                printf("\033[%d;%dHCoordonees : %d", 31, 20,tempsAQuaiD);
                printf("\033[%d;%dHCoordonees : %d", 32, 20,tempsAQuaiP);
            }
            if (monElementTrain->suivant == NULL) {
                monElementTrain = maListe.premier;
                tempsAQuaiD = tempsAQuai;
                tempsAQuai = tempsAQuaiP;
                time=(time+1)%monElementTrain->train.vitesse;
            }
            else{
                monElementTrain = maListe.dernier;
                tempsAQuaiP = tempsAQuai;
                tempsAQuai = tempsAQuaiD;
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
