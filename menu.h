void charger_menu(){
    FILE *fichier = fopen("menu_projet.txt", "r+");
    // Récupère le fichier et l'ouvre en mode lecture et écriture.
    // Le fichier doit exister.

    if (fichier == NULL){
        printf("Le fichier menu_projet.txt n'a pas pu être ouvert\n");
        return EXIT_FAILURE;
    }
    // Teste l'existence du fichier.

    int c;
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
