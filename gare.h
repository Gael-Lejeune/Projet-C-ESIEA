#include <stdio.h>
#include <stdlib.h>

typedef struct gare GARE;
struct gare{
    char custom[17][62];   /*Contient le train customisé, il faut choisirla bonne taille de votre tableau*/
};

GARE charger_gare(){
    FILE *fichier = fopen("txt/gare_test.txt", "r+");
    GARE magare;
    // magare.custom = (char**)malloc(62*sizeof(char *));
    // for(int i =0; i<62;i++) {
    // magare.custom[i] = (char*)malloc(17*sizeof(int));
    // printf("L%p\n", magare.custom[i]);
    // }

    if (fichier == NULL){
        printf("Le fichier gare_test.txt n'a pas pu être ouvert\n");
        // return EXIT_FAILURE;
    }
    // Teste l'existence du fichier.

    char c;
    // int i = 0;
    // int j = 0;
    // while(1) {
    //     c = fgetc(fichier);
    //     if (c != '\n') {
    //         magare.custom[i][j] = c;
    //         j++;
    //     }
    //     else{
    //         j = 0;
    //         i++;
    //     }
    //
    //     if( feof(fichier) ) {
    //         break;
    //     }
    //     printf("%c", c);
    // }

    for (size_t i = 0; i < 62; i++) {
        for (size_t j = 0; j < 17; j++) {
            c = fgetc(fichier);
            magare.custom[i][j] = c;
        }
    }
    fclose(fichier);

    for (int k = 0; k < sizeof(magare.custom); k++) {
        for (int l = 0; l < sizeof(magare.custom[k]); l++) {
            printf("%c", magare.custom[k][l]);
            fflush(stdout);
        }
        printf("\n");
    }

    // Ferme le fichier.
    return magare;
}
