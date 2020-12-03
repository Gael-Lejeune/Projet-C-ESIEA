#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include "utils.h"
#include "train.h"
#include "gare.h"
#include "voyageur.h"
#include "listeChainee.h"
#include "listeChaineeVoy.h"
// #include "voyageurind.h"


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
        printf("Le fichier menu.txt n'a pas pu être ouvert\n");
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


void deplacer_train(ELEMENT * monElementTrain, LISTE maListe, int tempsAQuai, FILE * train, GARE magare, int vitesse) {
    if (monElementTrain->train.direction == 'e') {
        //Si le train va vers l'est
        switch (monElementTrain->train.etat){
            //Suivant l'etat du train
            case 'd': //dehors
            // printf("TEST\n");
            if (monElementTrain->train.tempsAttente == 0){
                //Si le train a assez attendu hors gare
                monElementTrain->train.etat='e'; //Passage à l'état entrant
                if (vitesse == 1)
                system("play -q --no-show-progres -v 0.08 sound/freinage_metro.mp3 &");
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
                //Afficher les carcatères du train
                magare.custom[monElementTrain->train.lPortes-2][monElementTrain->train.posx - i] = monElementTrain->train.custom[0][monElementTrain->train.longueur - i];
                magare.custom[monElementTrain->train.lPortes-1][monElementTrain->train.posx - i] = monElementTrain->train.custom[1][monElementTrain->train.longueur - i];
                magare.custom[monElementTrain->train.lPortes][monElementTrain->train.posx - i] = monElementTrain->train.custom[2][monElementTrain->train.longueur - i];
                //Charger les trois lignes du train dans la matrice
            }
            if (monElementTrain->train.posx > 116) {
                //Si le train est entièrement en gare
                printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes+1, monElementTrain->train.posx - monElementTrain->train.longueur, "─");
                printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes - 1+1, monElementTrain->train.posx - monElementTrain->train.longueur, "|");
                printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes - 2+1, monElementTrain->train.posx - monElementTrain->train.longueur, "─");
                magare.custom[monElementTrain->train.lPortes-2][monElementTrain->train.posx - monElementTrain->train.longueur] = '-';
                magare.custom[monElementTrain->train.lPortes-1][monElementTrain->train.posx - monElementTrain->train.longueur] = '-';
                magare.custom[monElementTrain->train.lPortes][monElementTrain->train.posx - monElementTrain->train.longueur] = '-';

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
                        printf("\033[%d;%dH%c\n", monElementTrain->train.lPortes+1, monElementTrain->train.posx-i, ' '); //Remplacer l'affichage de ce caractère par un espace vide
                    }
                }
            }
            else if(monElementTrain->train.tempsAQuai-60 == 0){
                if (vitesse == 1) {
                    system(SONPORTES); //Son des portes qui se ferment
                }
            }
            else if (monElementTrain->train.tempsAQuai-10 == 0) {
                //Si le train part dans 5, fermer les portes
                for (int i = monElementTrain->train.posx; i >= 0; i--) {
                    if (monElementTrain->train.custom[2][monElementTrain->train.longueur - i] == 'f') {
                        printf("\033[%d;%dH%s%s%s\n", monElementTrain->train.lPortes+1, monElementTrain->train.posx-i, DOORCOLOR, "-", DEFAULT_COLOR);
                    }
                }
            }
            else if (monElementTrain->train.tempsAQuai == 0) {
                //Si le train doit partir
                monElementTrain->train.etat = 'p'; //Passer l'état du train à partant
                if (vitesse == 1) {
                    system(SONDEPART); //Son des portes qui se ferment
                }
            }
            monElementTrain->train.tempsAQuai--; //Décrémenter le temps d'attente en quai
            break;

            case 'p' : //Partant
            for (int i = monElementTrain->train.longueur; i >= 0; i--) {
                //Pour la longeur du train
                if (monElementTrain->train.posx - i < LONGUEUR) {
                    afficherCarTrain(monElementTrain->train.custom[0][monElementTrain->train.longueur - i], monElementTrain->train.lPortes-2, monElementTrain->train.posx-i);
                    afficherCarTrain(monElementTrain->train.custom[1][monElementTrain->train.longueur - i], monElementTrain->train.lPortes-1, monElementTrain->train.posx-i);
                    afficherCarTrain(monElementTrain->train.custom[2][monElementTrain->train.longueur - i], monElementTrain->train.lPortes, monElementTrain->train.posx-i);
                    magare.custom[monElementTrain->train.lPortes-2][monElementTrain->train.posx - i] = monElementTrain->train.custom[0][monElementTrain->train.longueur - i];
                    magare.custom[monElementTrain->train.lPortes-1][monElementTrain->train.posx - i] = monElementTrain->train.custom[1][monElementTrain->train.longueur - i];
                    magare.custom[monElementTrain->train.lPortes][monElementTrain->train.posx - i] = monElementTrain->train.custom[2][monElementTrain->train.longueur - i];
                    //Afficher et charger la partie du train encore en gare
                }
            }
            printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes+1, monElementTrain->train.posx - monElementTrain->train.longueur, "─");
            printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes - 1+1, monElementTrain->train.posx - monElementTrain->train.longueur, "|");
            printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes - 2+1, monElementTrain->train.posx - monElementTrain->train.longueur, "─");
            magare.custom[monElementTrain->train.lPortes-2][monElementTrain->train.posx - monElementTrain->train.longueur] = '-';
            magare.custom[monElementTrain->train.lPortes-1][monElementTrain->train.posx - monElementTrain->train.longueur] = '-';
            magare.custom[monElementTrain->train.lPortes][monElementTrain->train.posx - monElementTrain->train.longueur] = '-';
            // Afficher des rails derrière le train.
            monElementTrain->train.posx++; //Incrémenter la position du train
            if(monElementTrain->train.posx == monElementTrain->train.longueur + LONGUEUR+1){
                //Si le train est entièrement sorti
                suppD(&maListe); //Le supprimer de la liste
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
                //Si le train a assez attendu
                monElementTrain->train.etat='e'; //Passer à l'état "entrant"
                if (vitesse == 1)
                system(SONFREINAGE);

            }
            else{
                monElementTrain->train.tempsAttente--; //Decrementer le temps d'attente restant
            }
            break;

            case 'e': //Entrant
            for (int i = monElementTrain->train.posx; i < LONGUEUR; i++) {
                afficherCarTrain(monElementTrain->train.custom[0][i - monElementTrain->train.posx], monElementTrain->train.lPortes, i);
                afficherCarTrain(monElementTrain->train.custom[1][i - monElementTrain->train.posx], monElementTrain->train.lPortes + 1, i);
                afficherCarTrain(monElementTrain->train.custom[2][i - monElementTrain->train.posx], monElementTrain->train.lPortes + 2, i);
                magare.custom[monElementTrain->train.lPortes][i] = monElementTrain->train.custom[0][i - monElementTrain->train.posx];
                magare.custom[monElementTrain->train.lPortes+1][i] = monElementTrain->train.custom[1][i - monElementTrain->train.posx];
                magare.custom[monElementTrain->train.lPortes+2][i] = monElementTrain->train.custom[2][i - monElementTrain->train.posx];
                // Afficher et charger les caractères
            }
            if (monElementTrain->train.posx < 9) {
                printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes+1, monElementTrain->train.posx + monElementTrain->train.longueur+1, "─");
                printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes + 1+1, monElementTrain->train.posx + monElementTrain->train.longueur+1, "|");
                printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes + 2+1, monElementTrain->train.posx + monElementTrain->train.longueur+1, "─");
                magare.custom[monElementTrain->train.lPortes][monElementTrain->train.posx + monElementTrain->train.longueur] = '-';
                magare.custom[monElementTrain->train.lPortes+1][monElementTrain->train.posx + monElementTrain->train.longueur] = '-';
                magare.custom[monElementTrain->train.lPortes+2][monElementTrain->train.posx + monElementTrain->train.longueur] = '-';
                // Afficher et charger les caractères
            }
            monElementTrain->train.posx--;
            if(monElementTrain->train.posx == 5){
                //Si le train atteint sa position d'arret
                monElementTrain->train.etat = 's'; //Passer à stationné
            }
            break;
            case 's' : //Stationné
            if (tempsAQuai - 5 == monElementTrain->train.tempsAQuai) {
                //Si le train est la depui peu
                for (int i = 0; i <= monElementTrain->train.posx + monElementTrain->train.longueur; i++) {
                    if (monElementTrain->train.custom[0][i-1] == 'd') {
                        printf("\033[%d;%dH%c\n", monElementTrain->train.lPortes+1, monElementTrain->train.posx+i+1, ' ');
                    }
                } //Ouvrir les portes sur l'affichage
            }
            else if(monElementTrain->train.tempsAQuai-60 == 0){
                if (vitesse == 1) {
                    system(SONPORTES); //Son des portes qui se ferment
                }
            }
            else if (monElementTrain->train.tempsAQuai-10 == 0) {
                //Si part bientot
                for (int i = monElementTrain->train.posx; i <= monElementTrain->train.posx + monElementTrain->train.longueur; i++) {
                    if (monElementTrain->train.custom[0][i-1] == 'd') {
                        printf("\033[%d;%dH%s%s%s\n", monElementTrain->train.lPortes+1, monElementTrain->train.posx+i+1, DOORCOLOR, "-", DEFAULT_COLOR);
                    }
                }//Fermer les portes à nouveau
            }
            else if (monElementTrain->train.tempsAQuai == 0) {
                //Si le train a assez attendu à quai
                monElementTrain->train.etat = 'p'; //Passer à l'état "partant"
                if (vitesse == 1) {
                    system(SONDEPART); //Son des portes qui se ferment
                }
            }
            monElementTrain->train.tempsAQuai--; //Decrementer le temps à quai
            break;

            case 'p' : //Partant
            for (int i = monElementTrain->train.longueur; i >= 0; i--) {
                if (monElementTrain->train.posx + i >= 0) {
                    afficherCarTrain(monElementTrain->train.custom[0][i], monElementTrain->train.lPortes, monElementTrain->train.posx+i);
                    afficherCarTrain(monElementTrain->train.custom[1][i], monElementTrain->train.lPortes + 1, monElementTrain->train.posx+i);
                    afficherCarTrain(monElementTrain->train.custom[2][i], monElementTrain->train.lPortes + 2, monElementTrain->train.posx+i);
                    magare.custom[monElementTrain->train.lPortes][i] = monElementTrain->train.custom[0][i - monElementTrain->train.posx];
                    magare.custom[monElementTrain->train.lPortes+1][i] = monElementTrain->train.custom[1][i - monElementTrain->train.posx];
                    magare.custom[monElementTrain->train.lPortes+2][i] = monElementTrain->train.custom[2][i - monElementTrain->train.posx];
                }
            }//Afficher et charger
            printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes+1, monElementTrain->train.posx + monElementTrain->train.longueur+1, "─");
            printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes + 1+1, monElementTrain->train.posx + monElementTrain->train.longueur+1, "|");
            printf("\033[%d;%dH%s\n", monElementTrain->train.lPortes + 2+1, monElementTrain->train.posx + monElementTrain->train.longueur+1, "─");
            magare.custom[monElementTrain->train.lPortes][monElementTrain->train.posx + monElementTrain->train.longueur] = '-';
            magare.custom[monElementTrain->train.lPortes+1][monElementTrain->train.posx + monElementTrain->train.longueur] = '-';
            magare.custom[monElementTrain->train.lPortes+2][monElementTrain->train.posx + monElementTrain->train.longueur] = '-';
            monElementTrain->train.posx--;
            if(monElementTrain->train.posx == -(monElementTrain->train.longueur+1)){
                suppF(&maListe);
                ajoutF(&maListe, init_train(train, 'o', 180 +  rand() % 500));
            }
            break;
        }
    }
} //deplacer_train()


int main() {
    srand(time(NULL));
    while(1){
        system("clear");
        char c;
        afficher_fichier(FICMENU); //Affichage du menu de choix de mode
        char option = 1; //Mode choisi
        printf("\033[%d;%dH%s %s", 12, 60, "\033[0;m", "\033[0;m");
        printf("\033[%d;%dH%s▼%s", 12, 25, "\033[1;5m", "\033[0;m");
        printf("\033[%d;%dH", 40, 0); //Affichage de la fleche de selection
        while(1){
            c = key_pressed();
            if (c == 'q') {
                //Deplacer la fleche à gauche
                option = 1;
                printf("\033[%d;%dH%s %s", 12, 60, "\033[0;m", "\033[0;m");
                printf("\033[%d;%dH%s▼%s", 12, 25, "\033[1;5m", "\033[0;m");
                printf("\033[%d;%dH", 40, 0);
            }
            else if (c == 'd') {
                //Deplacer la fleche à droite
                option = 2;
                printf("\033[%d;%dH%s %s", 12, 25, "\033[0;m", "\033[0;m");
                printf("\033[%d;%dH%s▼%s", 12, 60, "\033[1;5m", "\033[0;m");
                printf("\033[%d;%dH", 40, 0);
            }
            else if (c == 'o'){
                //Demarrer la simulation
                break;
            }
            else if (c == 'k'){
                //Quitter la simulation
                system("clear");
                printf("\n\n\n\n\n\n\n\n\n\n\tMerci d'avoir utilisé notre simulateur. À bientôt !\n\n\n\n\n\n\n\n\n\n" );
                return 0;
                system("killall play 2> /dev/null &");

            }
            else if (c == 'l'){
                //Jouer un son de demo
                system(SONDEMO);
            }
            else if (c == 'm'){
                //Stopper le son
                system("killall play 2> /dev/null &");
            }
            else {
                printf("\033[%d;%dH ", 40, 0);
            }
        }

        GARE magare = init_gare(FICGARE); //Initialisation de la gare
        afficher_gare(magare); //Affichage de la gare


        if (option == 1) {
            //Si le mode choisi est le premier
            FILE * train = fopen(FICTRAIN, "r"); //Charger le fichier contenant le train


            LISTE maListeTrain; init_liste(&maListeTrain); //Création de la liste de train
            ajoutD(&maListeTrain,init_train(train, 'o', 180 + rand() % 200));
            ajoutD(&maListeTrain,init_train(train, 'e', 180 + rand() % 200)); //Ajout des deux trains

            printf("\033[%d;%dH%dm\n", 2, 2, maListeTrain.dernier->train.tempsAttente/60);
            printf("\033[%d;%dH%dm\n", 2, 122, maListeTrain.dernier->train.tempsAttente/60);
            printf("\033[%d;%dH%dm\n", 28, 2, maListeTrain.premier->train.tempsAttente/60);
            printf("\033[%d;%dH%dm\n", 28, 122, maListeTrain.premier->train.tempsAttente/60); //Affichage des temps d'attente

            printf("\033[%d;%dH%s\n", 30, 0, "Commandes possibles :");
            printf("\033[%d;%dH%s\n", 31, 0, "«k» -> menu");
            printf("\033[%d;%dH%s\n", 32, 0, "«+» -> accélère la simulation");
            printf("\033[%d;%dH%s\n", 33, 0, "«-» -> décélère la simulation");

            printf("\033[%d;%dH%s\n", 31, 80, "Vitesse de la simulation :");
            printf("\033[%d;%dHx1  \n", 32, 80);//Commandes possibles dans le simulateur


            int time = 0; //Temps
            int vitesse = 1; //Multiplicateur de vitesse de la simulation
            ELEMENT* monElementTrain = maListeTrain.premier; //Train
            char c=0; //Commande utilisateur

            while(1){
                if(time == 500){
                    c = key_pressed();
                    if (c == 'k') {
                        //Quitter la simulation
                        system("killall play 2> /dev/null &");
                        break;
                    } else if (c == '+') {
                        //Augmenter la vitesse
                        vitesse +=1;
                        printf("\033[%d;%dH%s%d  \n", 32, 80, "x", vitesse);

                    } else if (c == '-' && vitesse > 1) {
                        //Reduire la vitesse
                        vitesse -=1;
                        printf("\033[%d;%dH%s%d  \n", 32, 80, "x", vitesse);

                    }
                    if (maListeTrain.dernier->train.tempsAttente%60 == 59) {
                        //Si le nombre de secondes est égal à 59 au deuxieme train
                        if(maListeTrain.dernier->train.tempsAttente/60 == 0) {
                            //Si il reste moins d'une minute au deuxieme train
                            printf("\033[%d;%dH%s\n", 2, 2, "   "); //Ne rien afficher
                            printf("\033[%d;%dH%s\n", 2, 122, "   ");
                        }
                        else{
                            printf("\033[%d;%dH%dm\n", 2, 2, maListeTrain.dernier->train.tempsAttente/60); //Afficher le temps d'attente restant aux paneaux
                            printf("\033[%d;%dH%dm\n", 2, 122, maListeTrain.dernier->train.tempsAttente/60);
                        }
                    }
                    if (maListeTrain.premier->train.tempsAttente%60 == 59) {
                        //Si le nombre de secondes est égal à 59 au premier train
                        if(maListeTrain.premier->train.tempsAttente/60 == 0) {
                            //Si il reste moins d'une minute au premier train
                            printf("\033[%d;%dH%s\n", 28, 2, "   "); //Ne rien afficher
                            printf("\033[%d;%dH%s\n", 28, 122, "   ");
                        }
                        else {
                            printf("\033[%d;%dH%dm\n", 28, 2, maListeTrain.premier->train.tempsAttente/60);//Afficher le temps d'attente restant aux paneaux
                            printf("\033[%d;%dH%dm\n", 28, 122, maListeTrain.premier->train.tempsAttente/60);
                        }
                    }


                    deplacer_train(monElementTrain, maListeTrain, TEMPSAQUAI, train, magare, vitesse); //Deplacer le train

                    if (monElementTrain->suivant == NULL) {
                        //Si le train est le dernier de la liste
                        monElementTrain = maListeTrain.premier; //Selectionner le premier
                    }
                    else{
                        monElementTrain = maListeTrain.dernier; //Selectionner le suivant
                    }
                }
                time=(time+1)%(monElementTrain->train.vitesse/vitesse); //Incrémentation du temps
            }
            fclose(train); //Fermeture du fichier de train
        }

        else if (option == 2){
            //Si le mode 2 est choisi
            VOYAGEUR monvoyageur = init_voyageur(0, 93, '*', magare); //Initialisation du voyageur joué par le joueur

            LISTEV maListeV; init_listeV(&maListeV); //Initialisation de la liste de voyageurs

            ajoutVD(&maListeV, init_voyageurInd('*', magare, 1, 0)); //Ajout d'un voyageur dans la liste
            ELEMENTV * monElementVoy = maListeV.premier; //Selection du voyageur

            char movPlayer = 0; //Direction du joueur
            FILE * train = fopen(FICTRAIN, "r"); //Ouverture du fichier contenant le train
            LISTE maListeTrain; init_liste(&maListeTrain); //Liste de train
            ajoutD(&maListeTrain,init_train(train, 'o', 180 + rand() % 500));
            ajoutD(&maListeTrain,init_train(train, 'e', 180 + rand() % 500)); //Ajout des deux trains dans la liste
            printf("\033[%d;%dH%dm\n", 2, 2, maListeTrain.dernier->train.tempsAttente/60);
            printf("\033[%d;%dH%dm\n", 2, 122, maListeTrain.dernier->train.tempsAttente/60);
            printf("\033[%d;%dH%dm\n", 28, 2, maListeTrain.premier->train.tempsAttente/60);
            printf("\033[%d;%dH%dm\n", 28, 122, maListeTrain.premier->train.tempsAttente/60); //Affichage des temps d'attente sur les panneaux

            printf("\033[%d;%dH%s\n", 30, 0, "Commandes possibles :");
            printf("\033[%d;%dH%s\n", 31, 0, "«k» -> menu");
            printf("\033[%d;%dH%s\n", 32, 0, "«+» -> accélère la simulation");
            printf("\033[%d;%dH%s\n", 33, 0, "«-» -> décélère la simulation");
            printf("\033[%d;%dH%s\n", 34, 0, "«z» -> le voyageur monte");
            printf("\033[%d;%dH%s\n", 35, 0, "«s» -> le voyageur descend");
            printf("\033[%d;%dH%s\n", 36, 0, "«d» -> le voyageur va à droite");
            printf("\033[%d;%dH%s\n", 37, 0, "«q» -> le voyageur va à gauche");
            printf("\033[%d;%dH%s\n", 31, 80, "Vitesse de la simulation :");
            printf("\033[%d;%dHx1  \n", 32, 80); //Affichage des commandes possibles



            int timeTrain = 0; //Temps du train
            int vitesse = 1;
            int timeVoyageur = 0;

            ELEMENT* monElementTrain = maListeTrain.premier; //Selection du premier train

            while(1){
                if(timeTrain == 500){
                    if (movPlayer == 'k') {
                        //Si la touche k est préssée
                        system("killall play 2> /dev/null &");
                        break; //Fin de la simulation et retour au menu
                    }
                    else if (movPlayer == '+') {
                        //Accélération de la simulation
                        vitesse +=1;
                        printf("\033[%d;%dH%s%d  \n", 32, 80, "x", vitesse); //Affichage du multiplicateur de vitesse
                    }
                    else if (movPlayer == '-' && vitesse > 1) {
                        //Décélération de la simulation
                        vitesse -=1;
                        printf("\033[%d;%dH%s%d  \n", 32, 80, "x", vitesse);
                    }
                    movPlayer = key_pressed(); //Touche préssée
                    if (movPlayer != 0) {
                        //Si une touche est préssée
                        if((monvoyageur->posX == 0 && monvoyageur->posY < 62 && movPlayer == 'z') || (monvoyageur->posY == 124 && movPlayer == 'd') ){
                            //positions de sortie de gare
                            system("clear");
                            printf("\033[%d;%dHVous venez de sortir de la gare.", 15,20);
                            printf("\033[%d;%dHMerci d'avoir utilisé notre réseau !", 16,20);
                            fflush(stdout);
                            sleep(3);
                            break; //Sortie du programme
                            system("killall play 2> /dev/null &");
                        }
                        else if(monvoyageur->posX == 0 && monvoyageur->posY > 62 && movPlayer == 'z'){
                            //positions de changement de quai vers le bas
                            printf("\033[%d;%dH%c",monvoyageur->posX, monvoyageur->posY+1, ' ');
                            monvoyageur->posX = 24;
                            monvoyageur->posY = 0;
                            printf("\033[%d;%dHVous venez de changer de quai.",32, 80);
                            printf("\033[%d;%dHSortez de la gare à droite ou montez dans un train.",33, 80);
                        }
                        else if (monvoyageur->posY == 0 && movPlayer == 'q'){
                            //positions de changement de quai vers le haut
                            printf("\033[%d;%dH%c",monvoyageur->posX+1, monvoyageur->posY, ' ');
                            monvoyageur->posX = 0;
                            monvoyageur->posY = 93;
                            printf("\033[%d;%dHVous venez de changer de quai.",32, 80);
                            printf("\033[%d;%dHSortez de la gare à gauche ou montez dans un train.",33, 80);
                        }
                        mvtVoy(monvoyageur, magare, movPlayer);
                        if (monvoyageur->carpos == 'd' || monvoyageur->carpos == 'f') {
                            //Si le voyageur entre dans un train
                            system("clear");
                            printf("\033[%d;%dHVous avez pris le train. Bon voyage !", 15,20);
                            fflush(stdout);
                            sleep(3);
                            break;
                        }
                    }


                    if (maListeTrain.dernier->train.tempsAttente%60 == 59) {
                        //Affichage des temps d'attente (voir plus haut)
                        if(maListeTrain.dernier->train.tempsAttente/60 == 0) {
                            printf("\033[%d;%dH%s\n", 2, 2, "   ");
                            printf("\033[%d;%dH%s\n", 2, 122, "   ");
                        }
                        else if (maListeTrain.dernier->train.tempsAttente/60 >= 10){
                            printf("\033[%d;%dH%dm\n", 2, 2, maListeTrain.dernier->train.tempsAttente/60);
                            printf("\033[%d;%dH%dm\n", 2, 122, maListeTrain.dernier->train.tempsAttente/60);
                        }
                        else {
                            printf("\033[%d;%dH%dm \n", 2, 2, maListeTrain.dernier->train.tempsAttente/60);
                            printf("\033[%d;%dH%dm \n", 2, 122, maListeTrain.dernier->train.tempsAttente/60);
                        }
                    }
                    if (maListeTrain.premier->train.tempsAttente%60 == 59) {
                        if(maListeTrain.premier->train.tempsAttente/60 == 0) {
                            printf("\033[%d;%dH%s\n", 28, 2, "   ");
                            printf("\033[%d;%dH%s\n", 28, 122, "   ");
                        }
                        else if (maListeTrain.premier->train.tempsAttente/60 >= 10){
                            printf("\033[%d;%dH%dm\n", 28, 2, maListeTrain.premier->train.tempsAttente/60);
                            printf("\033[%d;%dH%dm\n", 28, 122, maListeTrain.premier->train.tempsAttente/60);
                        }
                        else {
                            printf("\033[%d;%dH%dm \n", 28, 2, maListeTrain.premier->train.tempsAttente/60);
                            printf("\033[%d;%dH%dm \n", 28, 122, maListeTrain.premier->train.tempsAttente/60);
                        }
                    }

                    deplacer_train(monElementTrain, maListeTrain, TEMPSAQUAI, train, magare, vitesse);


                    if (monElementTrain->train.etat == 's') {
                        //Si le train est stationné
                        if(monElementTrain->train.vide == 'v' && monElementTrain->train.direction == 'o'){
                            //Si letrain est vide et va vers l'ouest
                            monElementTrain->train.vide = 'p'; //Passer le train à "plein"
                            int nbvoytrain = rand()%16+2; //Randomizer le nombre de voyageurs dans le train
                            for(int i = nbvoytrain; i>0; i--){
                                //Créer les voyageurs
                                ajoutVD(&maListeV, init_voyageurInd('*', magare, 0, 'h'));
                            }
                        }
                        else if(monElementTrain->train.vide == 'v' && monElementTrain->train.direction == 'e'){
                            //Si le train est vide et va vers l'est
                            monElementTrain->train.vide = 'p';
                            int nbvoytrain = rand()%16+2;
                            for(int i = nbvoytrain; i>0; i--){
                                ajoutVD(&maListeV, init_voyageurInd('*', magare, 0, 'b'));
                            }
                        }
                        if(monElementTrain->train.tempsAQuai > 110 && (monElementTrain->train.tempsAQuai%5)==0){
                            //Si le train est à quai depuis peu et que son temps d'attente est divisible par 5
                            if(monElementTrain->train.direction == 'o'){
                                //S'il va vers l'ouest
                                ajoutVD(&maListeV, init_voyageurInd('*', magare, 0, 'h')); //Ajouter un voyageur sortant du train en haut
                            }
                            else {
                                //S'il va vers l'est
                                ajoutVD(&maListeV, init_voyageurInd('*', magare, 0, 'b')); //Ajouter un voyageur sortant du train en bas
                            }
                        }
                        monElementVoy = maListeV.premier; //Selectionner le premier voyageur
                        if (monElementTrain->train.direction == 'e' && monElementTrain->train.tempsAQuai > 10) {
                            //Si le train a ouvert ses portes
                            while (monElementVoy) {
                                //Pour tout les voyageurs
                                if (monElementVoy->voyageur->quai == 'b' && monElementVoy->voyageur->destinationX != monElementTrain->train.lPortes) {
                                    //Si le voyageur est en bas et a une destination autre que les portes
                                    monElementVoy->voyageur->destinationX = monElementTrain->train.lPortes; //Abcisse des portes
                                    monElementVoy->voyageur->destinationY = 59; //Valeur centrale de l'ordonnée des portes
                                    for (int i = 0; i <= LONGUEUR; i++) {
                                        //Pour la longueur de la matrice
                                        if (magare.custom[monElementTrain->train.lPortes][i] == 'f' && abs(i - monElementVoy->voyageur->posY) < abs(monElementVoy->voyageur->destinationY - monElementVoy->voyageur->posY) ) {
                                            //Si on observe une destination/porte plus proche que l'actuelle, remplacer
                                            monElementVoy->voyageur->destinationY = i;
                                        }
                                    }
                                }
                                monElementVoy = monElementVoy->suivant;
                            }
                        }
                        else if(monElementTrain->train.direction == 'o' && monElementTrain->train.tempsAQuai > 10){
                            //Pareil pour le train allant vers l'ouest
                            while (monElementVoy) {
                                if (monElementVoy->voyageur->quai == 'h' && monElementVoy->voyageur->destinationX != monElementTrain->train.lPortes) {
                                    monElementVoy->voyageur->destinationX = monElementTrain->train.lPortes;
                                    monElementVoy->voyageur->destinationY = 60;

                                    for (int i = 0; i <= LONGUEUR; i++) {
                                        if (magare.custom[monElementTrain->train.lPortes][i] == 'd' && abs(i - monElementVoy->voyageur->posY) < abs(monElementVoy->voyageur->destinationY - monElementVoy->voyageur->posY) ) {
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
                    else if (monElementTrain->train.etat == 'p') {
                        // Si le train est partant
                        monElementVoy = maListeV.premier;
                        while(monElementVoy){
                            //Pour tout les voyageurs
                            if (monElementVoy->voyageur->destinationX == monElementTrain->train.lPortes) {
                                //Si la destination du voyageur est une porte du train
                                if (monElementVoy->voyageur->quai == 'h') {
                                    //Definir au voyageur une destination aléatoire sur le quai ou il attend
                                    if(monElementVoy->voyageur->posY > 62){
                                        monElementVoy->voyageur->destinationY = rand()%60+62;
                                        monElementVoy->voyageur->destinationX = rand()%8+2;
                                    }
                                    else if(monElementVoy->voyageur->posY < 62){
                                        monElementVoy->voyageur->destinationY = rand()%60+2;
                                        monElementVoy->voyageur->destinationX = rand()%8+2;
                                    }
                                }
                                else {
                                    //Pour l'autre quai
                                    if(monElementVoy->voyageur->posY > 62){
                                        monElementVoy->voyageur->destinationY = rand()%60+62;
                                        monElementVoy->voyageur->destinationX = rand()%8+20;
                                    }
                                    else if(monElementVoy->voyageur->posY < 62){
                                        monElementVoy->voyageur->destinationY = rand()%60+2;
                                        monElementVoy->voyageur->destinationX = rand()%8+20;
                                    }
                                }
                            }
                            monElementVoy = monElementVoy->suivant; //Selectionner le voyageur suivant
                        }
                    }

                    if (monElementTrain->suivant == NULL) {
                        //Si le train actuel est le dernier de la liste
                        monElementTrain = maListeTrain.premier; //Selectionner le premier
                    }
                    else{
                        monElementTrain = maListeTrain.dernier; //Selectionner le suivant
                    }
                }
                timeTrain=(timeTrain+1)%(monElementTrain->train.vitesse/vitesse); //Incrémenter le temps du train
                if(timeVoyageur == 5000){
                    monElementVoy = maListeV.premier; //Selectionner le premier voyageur
                    int i = 1;
                    int mult = 1; //multiplicateur permettant de produire de meilleurs déplacements sur l'axe X
                    int posX;
                    while (monElementVoy) {
                        //Pour tout les voyageurs
                        if (monElementVoy->voyageur->destinationX == -1 && monElementVoy->voyageur->posX < 6 && monElementVoy->voyageur->posX > 2) {
                            //Si le voyageur a certaines position
                            mult = 5;
                        }
                        else if (monElementVoy->voyageur->destinationY == -1|| monElementVoy->voyageur->destinationY == LONGUEUR) {
                            mult = 10;
                        }
                        else {
                            mult = 1;
                        }
                        if (monElementVoy->voyageur->tempsAttente == 0) {
                            //Si le voyageur a assez attendu hors de la gare
                            if (abs(monElementVoy->voyageur->posX-monElementVoy->voyageur->destinationX)*mult > abs(monElementVoy->voyageur->posY-monElementVoy->voyageur->destinationY)) {
                                //Comparaison entre la destination et la position
                                if (monElementVoy->voyageur->posX > monElementVoy->voyageur->destinationX) {
                                    //Oppération sur l'axe X
                                    if (mvtVoy(monElementVoy->voyageur, magare, 'z') == 0) {
                                        //Si el voyageur ne peut aller vers el haut
                                        mvtVoy(monElementVoy->voyageur, magare, 'q'); //Aller à gauche
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
                                    if(mvtVoy(monElementVoy->voyageur, magare, 'd') == 0 && mvtVoy(monElementVoy->voyageur, magare, 's') == 0){
                                        mvtVoy(monElementVoy->voyageur, magare, 'z');
                                    }
                                }
                                else if(monElementVoy->voyageur->posY > monElementVoy->voyageur->destinationY){
                                    if(mvtVoy(monElementVoy->voyageur, magare, 'q') == 0 && mvtVoy(monElementVoy->voyageur, magare, 's') == 0){
                                        mvtVoy(monElementVoy->voyageur, magare, 'z');
                                    }
                                }
                            }
                            if(monElementVoy->voyageur->carpos == 'd' || monElementVoy->voyageur->carpos == 'f'){
                                //Si le voyageur entre dans une porte de train
                                suppV(&maListeV, monElementVoy); //Supprimer le voyageur
                            }
                        }
                        else if (monElementVoy->voyageur->tempsAttente == 1) {
                            //Si le voyageur a un temps d'attente égal à 1
                            ajoutVD(&maListeV, init_voyageurInd('*', magare, monElementVoy->voyageur->tempsAttente, 0)); //Ajouter une nouveau voyageur
                            monElementVoy->voyageur->tempsAttente -= 1;
                        }
                        else{
                            monElementVoy->voyageur->tempsAttente -= 1;
                        }
                        monElementVoy = monElementVoy->suivant;
                    }
                    i++;
                }
                timeVoyageur = (timeVoyageur +1)%(30000000/vitesse);
            }
            fclose(train);
        }
    }
    return 0;
}//main()
