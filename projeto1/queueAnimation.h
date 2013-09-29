#include "scenarioAnimation.h"

// Delivery Actions
#define DRAW  0
#define ERASE 1

typedef struct Queue {
    int length;
    int* queue;
} Queue;

/* Coloca uma pessoa na fila para entrar em um barco */
int enqueue(Queue* queue, int person, pthread_mutex_t* mutex_sail);

/* Tira uma pessoa da fila para entrar em um barco */
int dequeue(Queue* queue, int position, pthread_mutex_t* mutex_sail, pthread_cond_t* arrival_space);

/* Desenha a saída das pessoas de um barco no outro lado do rio */
void drawBoatDelivery(Boat* boat, pthread_mutex_t* mutex_sail);

/* Libera espaço para a saída das pessoas de um barco no outro lado do rio */
void eraseBoatDelivery(Boat* boat, pthread_mutex_t* mutex_sail);

