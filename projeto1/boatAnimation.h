#include "queueAnimation.h"

/* Desenha o barco parado com seus passageiros dentro */
void animateStoppedBoat(Boat* boat, pthread_mutex_t* mutex_sail);

/* Anima o barco em movimento com seus passageiros dentro (e o capitão remando) */
void animateBoatTravel(Boat* boat, pthread_mutex_t* mutex_sail);
