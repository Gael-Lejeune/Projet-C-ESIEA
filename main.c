#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
// #include <ncurses.h>
#include "menu.h"
#include "train.h"
#include "gare.h"

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

int main() {
    // char fichier[20] = "txt/menu.txt";
    charger_menu();

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
        afficher_gare(gare, magare);
        fclose(gare);
    }
    return 0;
}
