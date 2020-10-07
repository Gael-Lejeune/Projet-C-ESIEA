#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define couleur(param) printf("\033[%sm",param)


int APPARTIENT(char c, char alpha[]){
	int i, size, app=0;
	size=strlen(alpha);
	for(i=0; i<size;i++){
		if(c==alpha[i]){
			app=1; break;}	
	}
	return app;
}


int main(){
	FILE *fichier = fopen("/home/angelique/menu_projet.txt", "r+");
// Récupère le fichier et l'ouvre en mode lecture et écriture.
// Le fichier doit exister.

	int c;
	char alpha[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.,!:'";
	while(1) {
		c = fgetc(fichier);
		if(feof(fichier)) { 
			break ;
		}
		if(APPARTIENT(c,alpha)==1){couleur("36"); printf("%c",c);couleur("0");}
		else{printf("%c", c);}
	}
// récupère les éléments du fichier, un par un.

	fclose(fichier);
// Ferme le fichier.

	return 0;
}
