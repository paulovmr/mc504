#include "scenarioAnimation.h"

typedef struct Queue {
	int length;
	int* queue;
} Queue;

int enqueue(Queue* queue, int person, pthread_mutex_t* mutex_sail);

int dequeue(Queue* queue, int position, pthread_mutex_t* mutex_sail, pthread_cond_t* arrival_space);

void drawBoatDelivery(Boat* boat, pthread_mutex_t* mutex_sail);

void eraseBoatDelivery(Boat* boat, pthread_mutex_t* mutex_sail);
