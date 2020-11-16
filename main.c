#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
// #include <ncurses.h>
#include "train.h"
#include "gare.h"
#include "voyageur.h"
#include "listeChainee.h"
#include "voyageurind.h"

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
    else if (c == 'q'){
      system("clear");
      printf("\n\n\n\n\n\n\n\n\n\n\tMerci d'avoir utilisé notre simulateur. À bientôt !\n\n\n\n\n\n\n\n\n\n" );
      return 0;
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
    srand(time(NULL));
    ajoutD(&maListe,init_train(train, 'o', rand() % 500));
    fseek(train, 0, 0);
    ajoutD(&maListe,init_train(train, 'e', rand() % 500));
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
      if(time == 1000){
        printf("\033[%d;%dH%s\n", 2, 2, "   ");
        printf("\033[%d;%dH%s\n", 2, 122, "   ");
        printf("\033[%d;%dH%d\n", 2, 2, maListe.dernier->train.tempsAttente);
        printf("\033[%d;%dH%d\n", 2, 122, maListe.dernier->train.tempsAttente);
        printf("\033[%d;%dH%s\n", 28, 2, "   ");
        printf("\033[%d;%dH%s\n", 28, 122, "   ");
        printf("\033[%d;%dH%d\n", 28, 2, maListe.premier->train.tempsAttente);
        printf("\033[%d;%dH%d\n", 28, 122, maListe.premier->train.tempsAttente);
        // printf("\033[%d;%dH%s\n", lDoor - 2, monElementTrain->train.posx - monElementTrain->train.longueur - 1, "─");
        if (monElementTrain->train.direction == 'e') {
          lDoor = 18;
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
              afficherCarTrain(monElementTrain->train.custom[0][monElementTrain->train.longueur - i], lDoor-2, monElementTrain->train.posx-i);
              afficherCarTrain(monElementTrain->train.custom[1][monElementTrain->train.longueur - i], lDoor-1, monElementTrain->train.posx-i);
              afficherCarTrain(monElementTrain->train.custom[2][monElementTrain->train.longueur - i], lDoor, monElementTrain->train.posx-i);
            }
            if (monElementTrain->train.posx > 96) {
              printf("\033[%d;%dH%s\n", lDoor, monElementTrain->train.posx - monElementTrain->train.longueur - 1, "─");
              printf("\033[%d;%dH%s\n", lDoor - 1, monElementTrain->train.posx - monElementTrain->train.longueur - 1, "─");
              printf("\033[%d;%dH%s\n", lDoor - 2, monElementTrain->train.posx - monElementTrain->train.longueur - 1, "─");
            }
            monElementTrain->train.posx++;
            monElementTrain->train.vitesse = monElementTrain->train.vitesse + 50000; //décélération
            if(monElementTrain->train.posx == 102){
              monElementTrain->train.etat = 's';
            }
            break;

            case 's' :
            if (tempsAQuai == monElementTrain->train.tempsAQuai - 5) {
              for (int i = monElementTrain->train.posx; i >= 0; i--) {
                if (monElementTrain->train.custom[2][monElementTrain->train.longueur - i] == 'f') {
                  printf("\033[%d;%dH%c\n", lDoor, monElementTrain->train.posx-i-1, ' ');
                }
              }
            }
            else if (tempsAQuai-5 == 0) {
              for (int i = monElementTrain->train.posx; i >= 0; i--) {
                if (monElementTrain->train.custom[2][monElementTrain->train.longueur - i] == 'f') {
                  printf("\033[%d;%dH%s%s%s\n", lDoor, monElementTrain->train.posx-i-1, DOORCOLOR, "-", DEFAULT_COLOR);
                }
              }
            }
            else if (tempsAQuai == 0) {
              monElementTrain->train.etat = 'p';
            }

            tempsAQuai--;
            break;

            case 'p' :
            for (int i = monElementTrain->train.longueur; i >= 0; i--) {
              if (monElementTrain->train.posx - i <= LONGUEUR-1) {
                afficherCarTrain(monElementTrain->train.custom[0][monElementTrain->train.longueur - i], lDoor-2, monElementTrain->train.posx-i);
                afficherCarTrain(monElementTrain->train.custom[1][monElementTrain->train.longueur - i], lDoor-1, monElementTrain->train.posx-i);
                afficherCarTrain(monElementTrain->train.custom[2][monElementTrain->train.longueur - i], lDoor, monElementTrain->train.posx-i);
              }
            }
            printf("\033[%d;%dH%s\n", lDoor, monElementTrain->train.posx - monElementTrain->train.longueur - 1, "─");
            printf("\033[%d;%dH%s\n", lDoor - 1, monElementTrain->train.posx - monElementTrain->train.longueur - 1, "─");
            printf("\033[%d;%dH%s\n", lDoor - 2, monElementTrain->train.posx - monElementTrain->train.longueur - 1, "─");
            monElementTrain->train.posx++;
            monElementTrain->train.vitesse = monElementTrain->train.vitesse - 50000; //accélération
            if(monElementTrain->train.posx == monElementTrain->train.longueur + 1 + LONGUEUR){
              //monElementTrain->train.etat = 'r';
              suppd(&maListe);
              fseek(train, 0, 0);
              ajoutD(&maListe, init_train(train, 'e', rand() % 500));
              tempsAQuaiP = maListe.premier->train.tempsAQuai;
              tempsAQuai = maListe.premier->train.tempsAQuai;
            }

            break;

            // printf("test\n");
          }
        }

        else{
          lDoor = 12;
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
            for (int i = monElementTrain->train.posx; i <= LONGUEUR -1 ; i++) {
              afficherCarTrain(monElementTrain->train.custom[0][i - monElementTrain->train.posx], lDoor, i);
              afficherCarTrain(monElementTrain->train.custom[1][i - monElementTrain->train.posx], lDoor + 1, i);
              afficherCarTrain(monElementTrain->train.custom[2][i - monElementTrain->train.posx], lDoor + 2, i);
            }
            if (monElementTrain->train.posx < 30) {
              printf("\033[%d;%dH%s\n", lDoor, monElementTrain->train.posx + monElementTrain->train.longueur, "─");
              printf("\033[%d;%dH%s\n", lDoor + 1, monElementTrain->train.posx + monElementTrain->train.longueur, "─");
              printf("\033[%d;%dH%s\n", lDoor + 2, monElementTrain->train.posx + monElementTrain->train.longueur, "─");
            }
            monElementTrain->train.posx--;
            monElementTrain->train.vitesse = monElementTrain->train.vitesse + 40000; //décélération
            if(monElementTrain->train.posx == 25){
              monElementTrain->train.etat = 's';
            }
            break;
            case 's' :
            if (tempsAQuai == monElementTrain->train.tempsAQuai - 5) {
              printf("\033[%d;%dHCoordonees :           ", 36, 20);
              printf("\033[%d;%dHCoordonees : %d", 36, 20,monElementTrain->train.posx);

              for (int i = 0; i <= monElementTrain->train.posx + monElementTrain->train.longueur; i++) {
                if (monElementTrain->train.custom[0][i-1] == 'd') {
                  printf("\033[%d;%dH%c\n", lDoor, monElementTrain->train.posx+i, ' ');
                }
              }
            }
            else if (tempsAQuai-5 == 0) {
              for (int i = monElementTrain->train.posx; i <= monElementTrain->train.posx + monElementTrain->train.longueur; i++) {
                if (monElementTrain->train.custom[0][i-1] == 'd') {
                  printf("\033[%d;%dH%s%s%s\n", lDoor, monElementTrain->train.posx+i, DOORCOLOR, "-", DEFAULT_COLOR);
                }
              }
            }
            else if (tempsAQuai == 0) {
              monElementTrain->train.etat = 'p';
            }
            tempsAQuai--;
            break;

            case 'p' :
            for (int i = monElementTrain->train.longueur; i >= 0; i--) {
              if (monElementTrain->train.posx + i >= 0) {
                afficherCarTrain(monElementTrain->train.custom[0][i], lDoor, monElementTrain->train.posx+i);
                afficherCarTrain(monElementTrain->train.custom[1][i], lDoor + 1, monElementTrain->train.posx+i);
                afficherCarTrain(monElementTrain->train.custom[2][i], lDoor + 2, monElementTrain->train.posx+i);
              }
            }
            printf("\033[%d;%dH%s\n", lDoor, monElementTrain->train.posx + monElementTrain->train.longueur, "─");
            printf("\033[%d;%dH%s\n", lDoor + 1, monElementTrain->train.posx + monElementTrain->train.longueur, "─");
            printf("\033[%d;%dH%s\n", lDoor + 2, monElementTrain->train.posx + monElementTrain->train.longueur, "─");
            monElementTrain->train.posx--;
            monElementTrain->train.vitesse = monElementTrain->train.vitesse - 50000; //accélération
            if(monElementTrain->train.posx == -monElementTrain->train.longueur){
              // monElementTrain->train.etat = ' ';
              suppf(&maListe);
              fseek(train, 0, 0);
              ajoutF(&maListe, init_train(train, 'o', rand() % 500));
              tempsAQuaiD = maListe.dernier->train.tempsAQuai;
              tempsAQuai = maListe.dernier->train.tempsAQuai;
              //tempsAQuai = monElementTrain->train.tempsAQuai;
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
    int timer = 0;
    int timeInd = 0;
    VOYAGEUR monvoyageur = init_voyageur(0, 93, '*', magare);
    char movPlayer = 0;
    srand(time(NULL));
    INDVOYAGEUR voyageurInd1 = init_voyageurInd('*', magare, 'g');
    char movPlayerInd1 = 0;
    INDVOYAGEUR voyageurInd2 = init_voyageurInd('*', magare, 'd');
    char movPlayerInd2 = 0;
    INDVOYAGEUR voyageurInd3 = init_voyageurInd('*', magare, 'r');
    char movPlayerInd3 = 0;
    INDVOYAGEUR voyageurInd4 = init_voyageurInd('*', magare, 't');
    char movPlayerInd4 = 0;
    afficher_gare(magare);
    printf("\033[%d;%dHCoordonees : %d %d", 30, 20, monvoyageur->posX, monvoyageur->posY);
    while(1){
      if (timer == 500){
        movPlayer = key_pressed();
        if (movPlayer != 0) {
          mvtVoy(monvoyageur, magare, movPlayer);
          printf("\033[%d;%dHCoordonees : %d %d", 30, 20, monvoyageur->posX, monvoyageur->posY);
        }
      }
      timer = (timer + 2)%1000000;
      if(timeInd == 100000){
        movPlayerInd1 = mvtAleatoireVoy(voyageurInd1, 1);
        mvtVoyInd(voyageurInd1, magare, movPlayerInd1);
        movPlayerInd2 = mvtAleatoireVoy(voyageurInd2, 1);
        mvtVoyInd(voyageurInd2, magare, movPlayerInd2);
        movPlayerInd3 = mvtAleatoireVoy(voyageurInd3, 1);
        mvtVoyInd(voyageurInd3, magare, movPlayerInd3);
        movPlayerInd4 = mvtAleatoireVoy(voyageurInd4, 1);
        mvtVoyInd(voyageurInd4, magare, movPlayerInd4);
        printf("\033[%d;%dHCoordonees : %d %d", 30, 20, monvoyageur->posX, monvoyageur->posY);
      }
      timeInd = (timeInd +1)%30000000;
    }
    fclose(gare);
  }

  return 0;
}
