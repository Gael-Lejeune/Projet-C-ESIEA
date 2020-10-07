#include <stdio.h>

typedef struct gare GARE;
struct gare{
    char custom[62][17];   /*Contient le train customisé, il faut choisirla bonne taille de votre tableau*/
};

GARE charger_gare(){
    GARE magare;
    FILE *fichier = fopen("txt/gare_test.txt", "r+");
    // Récupère le fichier et l'ouvre en mode lecture et écriture.
    // Le fichier doit exister.

    if (fichier == NULL){
        printf("Le fichier gare_test.txt n'a pas pu être ouvert\n");
        // return EXIT_FAILURE;
    }
    // Teste l'existence du fichier.

    char c;
    for (int i = 0; i <= 62; i++) {
        for (int j = 0; j <= 17; j++) {
            c = fgetc(fichier);
            magare.custom[i][j] = c;
            if( feof(fichier) ) {
                break;
            }
            printf("%c", c);
        }
    }
    // récupère les éléments du fichier, un par un.

    fclose(fichier);
    // Ferme le fichier.
    return magare;
}
