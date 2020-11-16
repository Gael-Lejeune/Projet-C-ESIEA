#include "listeChainee.h"

void init_liste(LISTE *l)
{
    l->premier = NULL;
    l->dernier = NULL;
}

void ajoutF(LISTE *l, TRAIN t) //ajout d'élément en fin de liste
{
    ELEMENT * nouv = (ELEMENT*)malloc(sizeof(ELEMENT));
    if(!nouv) exit(EXIT_FAILURE);
    nouv->train = t;
    nouv->precedent = l->dernier;
    nouv->suivant = NULL;
    if(l->dernier) l->dernier->suivant = nouv;
    else l->premier = nouv;
    l->dernier = nouv;
}

void ajoutD(LISTE *l, TRAIN t) //ajoute au début
{
    ELEMENT * nouv = (ELEMENT*)malloc(sizeof(ELEMENT));
    if(!nouv) exit(EXIT_FAILURE);
    nouv->train = t;
    nouv->suivant = l->premier;
    nouv->precedent = NULL;
    if(l->premier) l->premier->precedent = nouv;
    else l->dernier = nouv;
    l->premier = nouv;
}

void suppF(LISTE *l) //supprime le dernier
{
    ELEMENT *tmp = l->dernier;
    l->dernier = tmp->precedent;
    if(l->dernier) l->dernier->suivant = NULL;
    else l->premier = NULL;
    free(tmp);
}

void suppD(LISTE *l) //supprime le premier
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
