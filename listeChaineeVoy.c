#include "listeChaineeVoy.h"

void init_listeV(LISTEV *l) //Initialisation de la liste chainée
{
    l->premier = NULL;
    l->dernier = NULL;
}

void ajoutVF(LISTEV *l, VOYAGEUR v) //ajout d'élément en fin de liste
{
    ELEMENTV * nouv = (ELEMENTV*)malloc(sizeof(ELEMENTV));
    if(!nouv) exit(EXIT_FAILURE);
    nouv->voyageur = v;
    nouv->precedent = l->dernier;
    nouv->suivant = NULL;
    if(l->dernier) l->dernier->suivant = nouv;
    else l->premier = nouv;
    l->dernier = nouv;
}

void ajoutVD(LISTEV *l, VOYAGEUR v) //ajoute au début
{
    ELEMENTV * nouv = (ELEMENTV*)malloc(sizeof(ELEMENTV));
    if(!nouv) exit(EXIT_FAILURE);
    nouv->voyageur = v;
    nouv->suivant = l->premier;
    nouv->precedent = NULL;
    if(l->premier) l->premier->precedent = nouv;
    else l->dernier = nouv;
    l->premier = nouv;
}

void suppVF(LISTEV *l) //supprime le dernier
{
    ELEMENTV *tmp = l->dernier;
    l->dernier = tmp->precedent;
    if(l->dernier) l->dernier->suivant = NULL;
    else l->premier = NULL;
    free(tmp);
}

void suppVD(LISTEV *l) //supprime le premier
{
    ELEMENTV *tmp = l->premier;
    l->premier = tmp->suivant;
    if(l->premier)l->premier->precedent = NULL;
    else l->dernier = NULL;
    free(tmp);
}

void suppV(LISTEV *l, ELEMENTV *voy) //supprime un élément quelle que soit sa position dans la liste
{
    if (voy->precedent == NULL) {
        suppVD(l);
    }
    else if (voy->suivant == NULL){
        suppVF(l);
    }
    else{
        ELEMENTV *tmp = l->premier;
        while(tmp){
            if(tmp == voy){
                tmp->precedent->suivant = tmp->suivant;
                tmp->suivant->precedent = tmp->precedent;
                break;
            }
            tmp = tmp->suivant;
        }
        free(tmp);
    }
}

void ClearV(LISTEV *l) //Vide la liste
{
    ELEMENTV *tmp;
    ELEMENTV *pelem = l->premier;
    while(pelem)
    {
        tmp = pelem;
        pelem = pelem->suivant;
        free(tmp);
    }
    l->premier = NULL;
    l->dernier = NULL;
}
