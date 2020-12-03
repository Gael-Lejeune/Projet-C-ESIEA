#include "listeChainee.h"

void init_liste_train(LISTE *l) //Initialisation de la liste chainée
{
    l->premier = NULL;
    l->dernier = NULL;
}

void ajout_train_fin(LISTE *l, TRAIN t) //Ajout d'élément en fin de liste
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

void ajout_train_debut(LISTE *l, TRAIN t) //Ajoute au début
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

void supp_train_fin(LISTE *l) //Supprime le dernier
{
    ELEMENT *tmp = l->dernier;
    l->dernier = tmp->precedent;
    if(l->dernier) l->dernier->suivant = NULL;
    else l->premier = NULL;
    free(tmp);
}

void supp_train_debut(LISTE *l) //Supprime le premier
{
    ELEMENT *tmp = l->premier;
    l->premier = tmp->suivant;
    if(l->premier)l->premier->precedent = NULL;
    else l->dernier = NULL;
    free(tmp);
}

void clear_train(LISTE *l) //Vide la liste
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
