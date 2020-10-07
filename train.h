typedef struct train TRAIN;
struct train{
    char direction ;    /*N => Nord, S => Sud, E => EST, O => OUEST*/
    int posx;           /*Position courante x de la tête du train*/
    int posy;           /*Position courante y de la tête du train*/
    int vitesse;        /*Vitesse du train*/
    char custom[150];   /*Contient le train customisé, il faut choisirla bonne taille de votre tableau*/
    char etat;          /*État du train => dehors, entrant, stationné,sortant, sorti*/
    int portes ;        /*Portes ouvertes ou fermées*/
    /*Vous pouvez bien-sur rajouter d’autres variables si nécessaire*/
    int longueur;       /*Longueur du train*/
};

void charger_train(){
    FILE *fichier = fopen("train_test.txt", "r+");
    // Récupère le fichier et l'ouvre en mode lecture et écriture.
    // Le fichier doit exister.

    if (fichier == NULL){
        printf("Le fichier menu_projet.txt n'a pas pu être ouvert\n");
        return EXIT_FAILURE;
    }
    // Teste l'existence du fichier.

    char c;
    int i;
    while(1) {
        c =+ fgetc(fichier);
        if( feof(fichier) ) {
            break ;
        }
    }
    // récupère les éléments du fichier, un par un.
    for (int i = 0; i < strlen(c); i++) {
        printf("%c", c);
    }

    fclose(fichier);
    // Ferme le fichier.


}
