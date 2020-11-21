#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include "train.h"
#include "gare.h"
#include "voyageur.h"
#include "listeChainee.h"
#include "listeChaineeVoy.h"
// #include "voyageurind.h"

#define PURPLE          "\033[1;35m"
#define CYAN            "\033[1;36m"
#define TRAINCOLOR      PURPLE
#define DOORCOLOR      	CYAN
#define DEFAULT_COLOR   "\033[0;m"
#define LONGUEUR 126
#define LARGEUR 29


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
} //key_pressed()

void afficher_fichier(char * nomfic){
    //Affiche un fichier passé en argument, ici, le menu de choix du programme
    FILE *fichier = fopen(nomfic, "r+"); // Récupère le fichier et l'ouvre en mode lecture .
    if (fichier == NULL){
        // Le fichier doit exister.
        printf("Le fichier menu_projet.txt n'a pas pu être ouvert\n");
        // return EXIT_FAILURE;
    } // Teste l'existence du fichier.

    char c;
    while(1) {
        c = fgetc(fichier);
        if(feof(fichier)) {
            break ;
        }
        printf("%c", c);
    } // récupère les éléments du fichier, un par un.

    fclose(fichier);// Ferme le fichier.
} //afficher_fichier()


void deplacer_train(ELEMENT * monElementTrain, LISTE maListe, int tempsAQuai, FILE * train) {
    if (monElementTrain->train.direction == 'e') {
        //Si le train va vers l'est
        switch (monElementTrain->train.etat){
            //Suivant l'etat du train
            case 'd': //dehors
            if (monElementTrain->train.tempsAttente == 0){
                //Si le train a assez attendu hors gare
                monElementTrain->train.etat='e'; //Passage à l'état entrant
            }
            else{
                //Sinon
                monElementTrain->train.tempsAttente--; //On décrémente le temps d'attente
            }
            break;

            case 'e': //Entrant
            for (int i = monElementTrain->train.posx; i >= 0; i--) {
                //Pour la partie du train entrée en gare
                afficherCarTrain(monElementTrain->train.custom[0][monElementTrain->train.longueur - i], monElementTrain->train.lPortes-2, monElementTrain->train.posx-i);
                afficherCarTrain(monElementTrain->train.custom[1][monElementTrain->train.longueur - i], monElementTrain->train.lPortes-1, monElementTrain->train.posx-i);
                afficherCarTrain(monElementTrain->train.custom[2][monElementTrain->train.longueur - i], monElementTrain->train.lPortes, monElementTrain->train.posx-i);
                //Afficher les trois lignes du train
            }
            if (monElementTrain->train.posx > 116) {
                //Si le train est entièrement en gare
                printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes, monElementTrain->train.posx - monElementTrain->train.longueur - 1, "─");
                printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes - 1, monElementTrain->train.posx - monElementTrain->train.longueur - 1, "─");
                printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes - 2, monElementTrain->train.posx - monElementTrain->train.longueur - 1, "─");
                //Afficher des rails derrière lui
            }
            monElementTrain->train.posx++; //Incrémenté la position du train
            if(monElementTrain->train.posx == 122){
                //Si le train a atteint sa destination de stationnement
                monElementTrain->train.etat = 's'; //Passer son état à "stationné"
            }
            break;

            case 's' : //Stationné
            if (tempsAQuai - 5 == monElementTrain->train.tempsAQuai) {
                //S'il est à quai depuis 5, ouvrir les portes
                for (int i = monElementTrain->train.posx; i >= 0; i--) {
                    //Pour la longueurdu train
                    if (monElementTrain->train.custom[2][monElementTrain->train.longueur - i] == 'f') {
                        //Si le caractère est une porte
                        printf("\033[%d;%dH%c\n", monElementTrain->train.lPortes, monElementTrain->train.posx-i-1, ' '); //Remplacer l'affichage de ce caractère par un espace vide
                    }
                }
            }
            else if (monElementTrain->train.tempsAQuai-5 == 0) {
                //Si le train part dans 5, fermer les portes
                for (int i = monElementTrain->train.posx; i >= 0; i--) {
                    if (monElementTrain->train.custom[2][monElementTrain->train.longueur - i] == 'f') {
                        printf("\033[%d;%dH%s%s%s\n", monElementTrain->train.lPortes, monElementTrain->train.posx-i-1, DOORCOLOR, "-", DEFAULT_COLOR);
                    }
                }
            }
            else if (monElementTrain->train.tempsAQuai == 0) {
                //Si le train doit partir
                monElementTrain->train.etat = 'p'; //Passer l'état du train à partant
            }
            monElementTrain->train.tempsAQuai--; //Décrémenter le temps d'attente en quai
            break;

            case 'p' : //Partant
            for (int i = monElementTrain->train.longueur; i >= 0; i--) {
                //Pour la longeur du train
                if (monElementTrain->train.posx - i <= LONGUEUR-1) {
                    afficherCarTrain(monElementTrain->train.custom[0][monElementTrain->train.longueur - i], monElementTrain->train.lPortes-2, monElementTrain->train.posx-i);
                    afficherCarTrain(monElementTrain->train.custom[1][monElementTrain->train.longueur - i], monElementTrain->train.lPortes-1, monElementTrain->train.posx-i);
                    afficherCarTrain(monElementTrain->train.custom[2][monElementTrain->train.longueur - i], monElementTrain->train.lPortes, monElementTrain->train.posx-i);
                    //Afficher la partie du train encore en gare
                }
            }
            printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes, monElementTrain->train.posx - monElementTrain->train.longueur - 1, "─");
            printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes - 1, monElementTrain->train.posx - monElementTrain->train.longueur - 1, "─");
            printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes - 2, monElementTrain->train.posx - monElementTrain->train.longueur - 1, "─");
            // Afficher des rails derrière le train.
            monElementTrain->train.posx++; //Incrémenter la position du train
            if(monElementTrain->train.posx == monElementTrain->train.longueur + 1 + LONGUEUR){
                //Si le train est entièrement sorti
                suppD(&maListe); //Le supprimer de la liste
                fseek(train, 0, 0); //Placer le pointeur au debut du fichier train
                ajoutD(&maListe, init_train(train, 'e', 180 +  rand() % 500)); //Ajouter un autre train à la place
            }
            break;
        }
    }

    else{
        //Si le train va vers l'ouest
        switch (monElementTrain->train.etat){
            case 'd': //dehors
            if (monElementTrain->train.tempsAttente == 0){
                monElementTrain->train.etat='e';
            }
            else{
                monElementTrain->train.tempsAttente--;
            }
            break;

            case 'e': //Entrant
            for (int i = monElementTrain->train.posx; i <= LONGUEUR -1 ; i++) {
                afficherCarTrain(monElementTrain->train.custom[0][i - monElementTrain->train.posx], monElementTrain->train.lPortes, i);
                afficherCarTrain(monElementTrain->train.custom[1][i - monElementTrain->train.posx], monElementTrain->train.lPortes + 1, i);
                afficherCarTrain(monElementTrain->train.custom[2][i - monElementTrain->train.posx], monElementTrain->train.lPortes + 2, i);
            }
            if (monElementTrain->train.posx < 10) {
                printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes, monElementTrain->train.posx + monElementTrain->train.longueur, "─");
                printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes + 1, monElementTrain->train.posx + monElementTrain->train.longueur, "─");
                printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes + 2, monElementTrain->train.posx + monElementTrain->train.longueur, "─");
            }
            monElementTrain->train.posx--;
            if(monElementTrain->train.posx == 5){
                monElementTrain->train.etat = 's';
            }
            break;
            case 's' : //Stationné
            if (tempsAQuai - 5 == monElementTrain->train.tempsAQuai) {
                printf("\033[%d;%dHCoordonees :           ", 36, 20);
                printf("\033[%d;%dHCoordonees : %d", 36, 20,monElementTrain->train.posx);

                for (int i = 0; i <= monElementTrain->train.posx + monElementTrain->train.longueur; i++) {
                    if (monElementTrain->train.custom[0][i-1] == 'd') {
                        printf("\033[%d;%dH%c\n", monElementTrain->train.lPortes, monElementTrain->train.posx+i, ' ');
                    }
                }
            }
            else if (monElementTrain->train.tempsAQuai-5 == 0) {
                for (int i = monElementTrain->train.posx; i <= monElementTrain->train.posx + monElementTrain->train.longueur; i++) {
                    if (monElementTrain->train.custom[0][i-1] == 'd') {
                        printf("\033[%d;%dH%s%s%s\n", monElementTrain->train.lPortes, monElementTrain->train.posx+i, DOORCOLOR, "-", DEFAULT_COLOR);
                    }
                }
            }
            else if (monElementTrain->train.tempsAQuai == 0) {
                monElementTrain->train.etat = 'p';
            }
            monElementTrain->train.tempsAQuai--;
            break;

            case 'p' : //Partant
            for (int i = monElementTrain->train.longueur; i >= 0; i--) {
                if (monElementTrain->train.posx + i >= 0) {
                    afficherCarTrain(monElementTrain->train.custom[0][i], monElementTrain->train.lPortes, monElementTrain->train.posx+i);
                    afficherCarTrain(monElementTrain->train.custom[1][i], monElementTrain->train.lPortes + 1, monElementTrain->train.posx+i);
                    afficherCarTrain(monElementTrain->train.custom[2][i], monElementTrain->train.lPortes + 2, monElementTrain->train.posx+i);
                }
            }
            printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes, monElementTrain->train.posx + monElementTrain->train.longueur, "─");
            printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes + 1, monElementTrain->train.posx + monElementTrain->train.longueur, "─");
            printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes + 2, monElementTrain->train.posx + monElementTrain->train.longueur, "─");
            monElementTrain->train.posx--;
            if(monElementTrain->train.posx == -monElementTrain->train.longueur){
                suppF(&maListe);
                fseek(train, 0, 0);
                ajoutF(&maListe, init_train(train, 'o', 180 +  rand() % 500));
            }

            break;

        }
    }
} //deplacer_train()


int main() {
    while(1){
        char c;
        // system("setterm -cursor on");
        // system("setterm -cursor off");
        system("clear");
        afficher_fichier("txt/menu_projet2.txt");
        char option = 1;
        printf("\033[%d;%dH%s %s", 12, 60, "\033[0;m", "\033[0;m");
        printf("\033[%d;%dH%s▼%s", 12, 25, "\033[1;5m", "\033[0;m");
        printf("\033[%d;%dH", 40, 0);
        while(1){
            c = key_pressed();
            if (c == 'q') {
                option = 1;
                printf("\033[%d;%dH%s %s", 12, 60, "\033[0;m", "\033[0;m");
                printf("\033[%d;%dH%s▼%s", 12, 25, "\033[1;5m", "\033[0;m");
                printf("\033[%d;%dH", 40, 0);
            }
            else if (c == 'd') {
                option = 2;
                printf("\033[%d;%dH%s %s", 12, 25, "\033[0;m", "\033[0;m");
                printf("\033[%d;%dH%s▼%s", 12, 60, "\033[1;5m", "\033[0;m");
                printf("\033[%d;%dH", 40, 0);
            }
            else if (c == 'o'){
                break;
            }
            else if (c == 'k'){
                system("clear");
                printf("\n\n\n\n\n\n\n\n\n\n\tMerci d'avoir utilisé notre simulateur. À bientôt !\n\n\n\n\n\n\n\n\n\n" );
                return 0;
            }
            else {
                printf("\033[%d;%dH ", 40, 0);
            }
        }

        printf("Vous avez choisi l'option %c\n", option);

        GARE magare = init_gare("txt/gare_test.txt");
        afficher_gare(magare);

        if (option == 1) {
            FILE * train = fopen("txt/train.txt", "r");
            LISTE maListe; init_liste(&maListe);
            srand(time(NULL));
            ajoutD(&maListe,init_train(train, 'o', 180 + rand() % 500));
            fseek(train, 0, 0);
            ajoutD(&maListe,init_train(train, 'e', 180 + rand() % 500));

            int time = 0;
            ELEMENT* monElementTrain = maListe.premier;
            int tempsAQuai = monElementTrain->train.tempsAQuai;
            char c=0;

            while(1){
                if(time == 500){
                    c = key_pressed();
                    if (c == 'k') {
                        break;
                    }
                    printf("\033[%d;%dH%s\n", 2, 2, "   ");
                    printf("\033[%d;%dH%s\n", 2, 122, "   ");
                    if (maListe.dernier->train.tempsAttente/60 != 0) {
                        printf("\033[%d;%dH%dm\n", 2, 2, maListe.dernier->train.tempsAttente/60);
                        printf("\033[%d;%dH%dm\n", 2, 122, maListe.dernier->train.tempsAttente/60);
                    }
                    printf("\033[%d;%dH%s\n", 28, 2, "   ");
                    printf("\033[%d;%dH%s\n", 28, 122, "   ");
                    if (maListe.premier->train.tempsAttente/60 != 0) {
                        printf("\033[%d;%dH%dm\n", 28, 2, maListe.premier->train.tempsAttente/60);
                        printf("\033[%d;%dH%dm\n", 28, 122, maListe.premier->train.tempsAttente/60);
                    }

                    deplacer_train(monElementTrain, maListe, tempsAQuai, train);

                    printf("\033[%d;%dHCoordonees :           ", 31, 20);
                    printf("\033[%d;%dHCoordonees :           ", 32, 20);
                    printf("\033[%d;%dHCoordonees : %d", 31, 20, maListe.dernier->train.tempsAQuai);
                    printf("\033[%d;%dHCoordonees : %d", 32, 20,maListe.premier->train.tempsAQuai);
                    if (monElementTrain->suivant == NULL) {
                        monElementTrain = maListe.premier;
                    }
                    else{
                        monElementTrain = maListe.dernier;
                    }
                }
                time=(time+1)%monElementTrain->train.vitesse;
            }
            fclose(train);
        }

        else if (option == 2){
            VOYAGEUR monvoyageur = init_voyageur(2, 93, '*', magare);

            LISTEV maListeV; init_listeV(&maListeV);
            srand(time(NULL));
            ajoutVD(&maListeV, init_voyageurInd('*', magare, 1));
            printf("\033[%d;%dH%d", 40, 40, maListeV.premier->voyageur->tempsAttente);
            // ajoutVD(&maListeV, init_voyageurInd('*', magare, 5));
            // ajoutVD(&maListeV, init_voyageurInd('*', magare, 10));
            // ajoutVD(&maListeV, init_voyageurInd('*', magare, 15));
            ELEMENTV * monElementVoy = maListeV.premier;
            // afficher_voyageur(monvoyageur);

            char movPlayer = 0;
            FILE * train = fopen("txt/train.txt", "r");
            LISTE maListe; init_liste(&maListe);
            // srand(time(NULL));
            ajoutD(&maListe,init_train(train, 'o', 180 + rand() % 500));
            fseek(train, 0, 0);
            ajoutD(&maListe,init_train(train, 'e', 180 + rand() % 500));

            int timer = 0;
            int timeInd = 0;

            ELEMENT* monElementTrain = maListe.premier;
            int tempsAQuai = monElementTrain->train.tempsAQuai;

            while(1){
                if(timer == 500){
                    if (movPlayer == 'k') {
                        break;
                    }
                    movPlayer = key_pressed();
                    if (movPlayer != 0) {
                        // printf("test\n");
                        mvtVoy(monvoyageur, magare, movPlayer);
                        printf("\033[%d;%dHCoordonees : %d %d", 36, 20, monvoyageur->posX, monvoyageur->posY);
                        if(monvoyageur->posX == 0 || monvoyageur->posY == 0 || monvoyageur->posY == 124){ //positions de sortie de gare
                            // printf("\033[%d;%dHBravo, vous êtes sorti !", 20, 20);
                            // system("clear");
                            // usleep(1000000);
                            movPlayer = 'k';
                        }
                    }

                    printf("\033[%d;%dH%s\n", 2, 2, "   ");
                    printf("\033[%d;%dH%s\n", 2, 122, "   ");
                    if (maListe.dernier->train.tempsAttente/60 != 0) {
                        printf("\033[%d;%dH%dm\n", 2, 2, maListe.dernier->train.tempsAttente/60);
                        printf("\033[%d;%dH%dm\n", 2, 122, maListe.dernier->train.tempsAttente/60);
                    }
                    printf("\033[%d;%dH%s\n", 28, 2, "   ");
                    printf("\033[%d;%dH%s\n", 28, 122, "   ");
                    if (maListe.premier->train.tempsAttente/60 != 0) {
                        printf("\033[%d;%dH%dm\n", 28, 2, maListe.premier->train.tempsAttente/60);
                        printf("\033[%d;%dH%dm\n", 28, 122, maListe.premier->train.tempsAttente/60);
                    }

                    deplacer_train(monElementTrain, maListe, tempsAQuai, train);
                    if (monElementTrain->train.etat == 's') {
                        monElementVoy = maListeV.premier;
                        if (monElementTrain->train.direction == 'e') {
                            while (monElementVoy) {
                                if (monElementVoy->voyageur->quai == 'b') {
                                    monElementVoy->voyageur->destinationX = 18;
                                    for (int i = 0; i <= monElementTrain->train.posx + monElementTrain->train.longueur; i++) {
                                        if (monElementTrain->train.custom[0][i-1] == 'd' && abs(i - monElementVoy->voyageur->posY) < abs(monElementVoy->voyageur->destinationY - monElementVoy->voyageur->posY) ) {
                                            monElementVoy->voyageur->destinationY = i;
                                        }
                                    }
                                }
                                monElementVoy = monElementVoy->suivant;
                            }
                        }
                        else {
                            while (monElementVoy) {
                                if (monElementVoy->voyageur->quai == 'h') {
                                    monElementVoy->voyageur->destinationX = 10;
                                    monElementVoy->voyageur->destinationX = 62;

                                    for (int i = monElementTrain->train.posx; i <= monElementTrain->train.posx + monElementTrain->train.longueur; i++) {
                                        if (monElementTrain->train.custom[0][i-1] == 'd' && abs(i - monElementVoy->voyageur->posY) < abs(monElementVoy->voyageur->destinationY - monElementVoy->voyageur->posY) ) {
                                            monElementVoy->voyageur->destinationY = i;
                                        }
                                    }
                                }
                                if (monElementVoy->suivant == NULL)
                                break;
                                monElementVoy = monElementVoy->suivant;
                            }
                        }
                    }
                    if (monElementTrain->suivant == NULL) {
                        monElementTrain = maListe.premier;
                    }
                    else{
                        monElementTrain = maListe.dernier;
                    }
                }
                timer=(timer+1)%monElementTrain->train.vitesse;
                if(timeInd == 5000){
                    monElementVoy = maListeV.premier;
                    int i = 1;
                    while (monElementVoy) {
                        if (monElementVoy->voyageur->tempsAttente == 0) {
                            if (abs(monElementVoy->voyageur->posX-monElementVoy->voyageur->destinationX) > abs(monElementVoy->voyageur->posY-monElementVoy->voyageur->destinationY)) {
                                if (monElementVoy->voyageur->posX > monElementVoy->voyageur->destinationX) {
                                    if (mvtVoy(monElementVoy->voyageur, magare, 'z') == 0) {
                                        mvtVoy(monElementVoy->voyageur, magare, 'd');
                                    }
                                }
                                else if(monElementVoy->voyageur->posX < monElementVoy->voyageur->destinationX){
                                    if(mvtVoy(monElementVoy->voyageur, magare, 's') == 0){
                                        mvtVoy(monElementVoy->voyageur, magare, 'q');
                                    }
                                }
                            }
                            else {
                                if (monElementVoy->voyageur->posY < monElementVoy->voyageur->destinationY) {
                                    if(mvtVoy(monElementVoy->voyageur, magare, 'd') == 0){
                                        mvtVoy(monElementVoy->voyageur, magare, 's');
                                    }
                                }
                                else if(monElementVoy->voyageur->posY > monElementVoy->voyageur->destinationY){
                                    if(mvtVoy(monElementVoy->voyageur, magare, 'q') == 0){
                                        mvtVoy(monElementVoy->voyageur, magare, 's');
                                    }
                                }
                            }
                        }
                        else if (monElementVoy->voyageur->tempsAttente == 1) {
                            ajoutVD(&maListeV, init_voyageurInd('*', magare, monElementVoy->voyageur->tempsAttente));
                            monElementVoy->voyageur->tempsAttente -= 1;
                        }
                        else{
                            monElementVoy->voyageur->tempsAttente -= 1;
                        }
                        monElementVoy = monElementVoy->suivant;
                    }
                    i++;
                }
                timeInd = (timeInd +1)%30000000;
            }
            fclose(train);
        }
    }
    return 0;
}//main()
