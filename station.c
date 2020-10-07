#include <stdio.h>
#include <stdlib.h>


int main(){
	FILE *fichier = fopen("/home/angelique/station_projet.txt", "r+");
// Récupère le fichier et l'ouvre en mode lecture et écriture.
// Le fichier doit exister.

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

	return 0;
}
