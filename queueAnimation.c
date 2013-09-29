#include "queueAnimation.h"

void erasePerson(int leftRight, int position, pthread_mutex_t* mutex_sail) {
	int x = 2 + 4 * (position % 2) + leftRight * 100;
	int y = 1 + 3 * (position / 2);
	
	pthread_mutex_lock(mutex_sail);
	
	gotoxy(x, y); printf(GRASS "   ");
	gotoxy(x, y + 1); printf("   ");
	gotoxy(x, y + 2); printf("   ");
	
	printf(FG_WHITE BG_BLACK);
	flush();
	
	pthread_mutex_unlock(mutex_sail);
}

void drawPerson(int leftRight, int position, int person, pthread_mutex_t* mutex_sail) {
	int x = 2 + 4 * (position % 2) + leftRight * 100;
	int y = 1 + 3 * (position / 2);
	
	pthread_mutex_lock(mutex_sail);
	
	gotoxy(x, y); 
	if (person == MICROSOFT_EMPLOYEE) {
		printf(GRASS MICROSOFT_EMPLOYEE_COLOR " O ");
	} else {
		printf(GRASS LINUX_HACKER_COLOR " O ");
	}
	
	gotoxy(x, y + 1); printf(GRASS FG_BLACK "/|\\");
	gotoxy(x, y + 2); printf("/ \\");
	
	printf(FG_WHITE BG_BLACK);
	flush();
	
	pthread_mutex_unlock(mutex_sail);
}

int enqueue(Queue* queue, int person, pthread_mutex_t* mutex_sail) {
	int i;
	
	for (i = 0; i < queue->length; i++) {
		if (queue->queue[i] == -1) {
			queue->queue[i] = person;
			drawPerson(0, i, person, mutex_sail);
			sleep(1);
			return i;
		}
	}
	
	return -1;
}

int dequeue(Queue* queue, int position, pthread_mutex_t* mutex_sail) {
	int result = queue->queue[position];
	queue->queue[position] = -1;
	erasePerson(0, position, mutex_sail);
	
	return result;
}

void drawBoatDelivery(Boat* boat, pthread_mutex_t* mutex_sail) {
	int i;
	
	for (i = 0; i < boat->qtd; i++) {
		drawPerson(1, 4 * boat->position + i, boat->people[i], mutex_sail);
	}
}

void eraseBoatDelivery(Boat* boat, pthread_mutex_t* mutex_sail) {
	int i;
	
	for (i = 0; i < boat->qtd; i++) {
		erasePerson(1, 4 * boat->position + i, mutex_sail);
	}
}

