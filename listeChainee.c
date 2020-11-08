#include <stdio.h>
#include <stdlib.h>
#include "train.c"


#define LARGT 2
#define LONGT 52

typedef struct  Element ELEMENT;
struct Element{
    TRAIN train;
    ELEMENT *precedent;
    ELEMENT *suivant; //permet de lier les éléments entre eux
};

typedef struct Liste LISTE  ;
struct Liste
{
    ELEMENT *premier;
    ELEMENT *dernier;
};

void Init(LISTE *l)
{
    l->premier = NULL;
    l->dernier = NULL;
}

void Ajoutf(LISTE *l, TRAIN t) //ajout d'élément en fin de liste
{
    ELEMENT * nouv = (ELEMENT*)malloc(sizeof(ELEMENT*));
    if(!nouv) exit(EXIT_FAILURE);
    nouv->train = t;
    nouv->precedent = l->dernier;
    nouv->suivant = NULL;
    if(l->dernier) l->dernier->suivant = nouv;
    else l->premier = nouv;
    l->dernier = nouv;
}

void Ajoutd(LISTE *l, TRAIN t) //ajoute au début
{
    ELEMENT * nouv = (ELEMENT*)malloc(sizeof(ELEMENT*));
    if(!nouv) exit(EXIT_FAILURE);
    nouv->train = t;
    nouv->suivant = l->premier;
    nouv->precedent = NULL;
    if(l->premier) l->premier->precedent = nouv;
    else l->dernier = nouv;
    l->premier = nouv;
}

void Suppf(LISTE *l) //supprime le dernier
{
    ELEMENT *tmp = l->dernier;
    l->dernier = tmp->precedent;
    if(l->dernier) l->dernier->suivant = NULL;
    else l->premier = NULL;
    free(tmp);
}

void Suppd(LISTE *l) //supprime le premier
{
    ELEMENT *tmp = l->premier;
    l->premier = tmp->suivant;
    if(l->premier)l->premier->precedent = NULL;
    else l->dernier = NULL;
    free(tmp);
}

void View(LISTE l)
{
    ELEMENT *pelem = l.premier;
    while(pelem)
    {
        printf("%c\n",pelem->train.direction);
        pelem = pelem->suivant;
    }
}

void Clear(LISTE *l)
{
    ELEMENT *tmp;
    ELEMENT *pelem = l->premier;
    while(pelem)
    {
        tmp = pelem;
        pelem = pelem->suivant;
        free(tmp);
    }
    l->premier = NULL;
    l->dernier = NULL;
}

int main(){
    LISTE MaListe;
    FILE * train = fopen("txt/train_test.txt", "r");
    TRAIN montrain1 = init_train(train, 'e');
    TRAIN montrain2 = init_train(train, 'o');
    Init(&MaListe);
    Ajoutd(&MaListe,montrain1);
    Ajoutf(&MaListe,montrain2);
    Ajoutf(&MaListe,montrain1);
    Ajoutd(&MaListe,montrain2);
    View(MaListe);
    printf("__________\n");
    Suppd(&MaListe);
    Suppd(&MaListe);
    Suppf(&MaListe);
    View(MaListe);
    printf("\n__________");
    Ajoutf(&MaListe,montrain2);
    Suppd(&MaListe);
    Suppd(&MaListe);
    View(MaListe);
    printf("\n__________");
    Clear(&MaListe);
    View(MaListe);
    return 0;
}
