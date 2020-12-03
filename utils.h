#ifndef H_UTILS
#define H_UTILS


//VALEURS DE LA GARE
#define LONGUEUR 125
#define LARGEUR 29


//COULEURS
#define BLACK           "\033[1;30m"
#define RED             "\033[1;31m"
#define GREEN           "\033[1;32m"
#define YELLOW          "\033[1;33m"
#define BLUE            "\033[1;34m"
#define PURPLE          "\033[1;35m"
#define CYAN            "\033[1;36m"
#define GREY            "\033[1;37m"
#define BLINK           "\033[1;5m"
#define DEFAULT_COLOR   "\033[0;m"
#define WALLCOLOR       RED
#define RAILCOLOR       GREY
#define PLAYERCOLOR     YELLOW
#define TRAINCOLOR      PURPLE
#define DOORCOLOR      	CYAN

//VALEURS DU TRAIN
#define TEMPSAQUAI      250
#define VITESSETRAIN    5000000

#define LARGT           3
#define LONGT           116

//FICHIERS
#define FICTRAIN        "txt/train.txt"
#define FICGARE         "txt/gare.txt"
#define FICMENU         "txt/menu.txt"

//SONS
#define SONDEMO         "play -q --no-show-progres -v 0.08 sound/demo.mp3 &"
#define SONDEPART       "play -q --no-show-progres -v 0.08 sound/depart_metro.mp3 &"
#define SONFREINAGE     "play -q --no-show-progres -v 0.08 sound/freinage_metro.mp3 &"
#define SONPORTES       "play -q --no-show-progres -v 0.08 sound/porte_metro.mp3 &"


#endif
