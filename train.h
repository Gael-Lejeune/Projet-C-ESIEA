#include <stdio.h>

typedef struct train TRAIN;
struct train{
    char direction ;    /*N => Nord, S => Sud, E => EST, O => OUEST*/
    int posx;           /*Position courante x de la tête du train*/
    int posy;           /*Position courante y de la tête du train*/
    int vitesse;        /*Vitesse du train*/
    char custom[2][52];   /*Contient le train customisé, il faut choisirla bonne taille de votre tableau*/
    char etat;          /*État du train => dehors, entrant, stationné,sortant, sorti*/
    int portes;        /*Portes ouvertes ou fermées*/
    /*Vous pouvez bien-sur rajouter d’autres variables si nécessaire*/
    int longueur;       /*Longueur du train*/
};

void charger_train(){
    FILE *fichier = fopen("txt/train_test.txt", "r+");
    // Récupère le fichier et l'ouvre en mode lecture et écriture.
    // Le fichier doit exister.

    if (fichier == NULL){
        printf("Le fichier train_test.txt n'a pas pu être ouvert\n");
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
