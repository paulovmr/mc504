/*
 * Lab 01
 * Guilherme Henrique Nunes - RA 117108
 * Paulo Vitor Martins do Rego - RA 118343
 * Thiago Pires de Oliveira - RA 123153
 */

#include "boatAnimation.h"

sem_t hackers_queue, serfs_queue;
pthread_mutex_t mutex;
pthread_mutex_t mutex_sail;
pthread_mutex_t atomic_hackers;
pthread_mutex_t atomic_serfs;
pthread_mutex_t arrival;
pthread_cond_t arrival_space;

int hackers = 0;
int serfs = 0;
int boats = 1;
int boatCapacity = 4;
int halfBoatCapacity = 2;
int lastBoatSailed = 0;
Boat** fleet;
Queue* queue;

int aux = 0;

Boat* newBoat(int position){
    Boat* x = malloc (sizeof *x);
	x->position = position;
	x->x = 11;
	x->y = 2 + (position * BOAT_HEIGHT);
	x->status = 0;
	x->capacity = boatCapacity;
	x->qtd = 0;
	x->people = malloc(boatCapacity * sizeof(int));
    x->hackers = 0;
    x->serfs = 0;
    x->isSailing = 0;
    pthread_barrier_init(&(x->barrier), NULL, boatCapacity);
    
    return x;
}

void atomic_inc_aux() {
    pthread_mutex_lock(&atomic_hackers);
    aux++;
    pthread_mutex_unlock(&atomic_hackers);
}

void atomic_inc_hackers() {
    pthread_mutex_lock(&atomic_hackers);
    hackers++;
    pthread_mutex_unlock(&atomic_hackers);
}

void atomic_dec_hackers() {
    pthread_mutex_lock(&atomic_hackers);
    hackers--;
    pthread_mutex_unlock(&atomic_hackers);
}

void atomic_inc_serfs() {
    pthread_mutex_lock(&atomic_serfs);
    serfs++;
    pthread_mutex_unlock(&atomic_serfs);
}

void atomic_dec_serfs() {
    pthread_mutex_lock(&atomic_serfs);
    serfs--;
    pthread_mutex_unlock(&atomic_serfs);
}

int threadArrival(int i){
	int position = -1;
	pthread_mutex_lock(&arrival);
	while (position == -1) {
		position = enqueue(queue, i, &mutex_sail);
		if (position == -1) {
			pthread_cond_wait(&arrival_space, &arrival);
		}
    }
    pthread_mutex_unlock(&arrival);
    
    return position;
}

void board(int person, int i, int position){
	Boat* boat = fleet[i];
	
	dequeue(queue, position, &mutex_sail, &arrival_space);
	
	boat->people[boat->qtd++] = person;
	
    animateStoppedBoat(boat, &mutex_sail);
    
    if (boat->qtd != boat->capacity) {
		sleep(1);
	}
}

void rowBoat(int i){
	Boat* boat = fleet[i];

    lastBoatSailed = i;

    animateBoatTravel(boat, &mutex_sail);    
    boat->isSailing = 0;
}

void freeWaitings(){
    int j;

    for(j = 0; j < serfs; j++) sem_post(&serfs_queue);
    for(j = 0; j < hackers; j++) sem_post(&hackers_queue);
}

void log(int position, int lockUnlock) {
	/*int i;
    pthread_mutex_lock(&mutex_sail);
	atomic_inc_aux();
	i = aux;
	gotoxy(1, 20 + lockUnlock + 2* position); printf("Thread in position %d - LockUnlock: %d - aux: %d", position, lockUnlock, i); flush();
	pthread_mutex_unlock(&mutex_sail);*/
}

void *f_thread_hacker() {
    
    int i, position, firstBoatToCheck, lastBoatToCheck;

    position = threadArrival(LINUX_HACKER);
    atomic_inc_hackers();

    while (1) {
		pthread_mutex_lock(&mutex);
		log(position, 0);
		
		firstBoatToCheck = lastBoatSailed;
		lastBoatToCheck = boats;
		
        for (i = firstBoatToCheck; i < lastBoatToCheck; i++) {
            if (!(
                   (fleet[i]->serfs + fleet[i]->hackers == boatCapacity) || (fleet[i]->isSailing)
			 || ((fleet[i]->hackers == halfBoatCapacity) && (fleet[i]->serfs == (halfBoatCapacity - 1))) || (fleet[i]->serfs == (halfBoatCapacity + 1))
                 )
            ) {
                fleet[i]->hackers++;
                board(LINUX_HACKER, i, position);
                if (fleet[i]->hackers == boatCapacity) {
                    fleet[i]->isSailing = 1;
                    fleet[i]->hackers = 0;
                    pthread_barrier_wait(&(fleet[i]->barrier));
    				log(position, 1);
                    pthread_mutex_unlock(&mutex);
                    rowBoat(i);
                    freeWaitings();
                } else if ((fleet[i]->hackers == halfBoatCapacity)&&(fleet[i]->serfs == halfBoatCapacity)) {
                    fleet[i]->isSailing = 1;
                    fleet[i]->hackers = 0;
                    fleet[i]->serfs = 0;
                    pthread_barrier_wait(&(fleet[i]->barrier));
    				log(position, 1);
                    pthread_mutex_unlock(&mutex);
                    rowBoat(i);
                    freeWaitings();
                } else {
    				log(position, 1);
                    pthread_mutex_unlock(&mutex);
                    pthread_barrier_wait(&(fleet[i]->barrier));
                }
                
				atomic_dec_hackers();
				
                return NULL;
            }
            
			if (i == boats - 1 && firstBoatToCheck != 0) {
				lastBoatToCheck = firstBoatToCheck;
				i = -1;
			}
        }
        
		log(position, 1);
        pthread_mutex_unlock(&mutex);
        sem_wait(&hackers_queue);
    }  

	atomic_dec_hackers();
    
    return NULL;
}

void *f_thread_serf() {

    int i, position, firstBoatToCheck, lastBoatToCheck;

    position = threadArrival(MICROSOFT_EMPLOYEE);
	atomic_inc_serfs();

    while (1) {
		pthread_mutex_lock(&mutex);
		log(position, 0);
		
        firstBoatToCheck = lastBoatSailed;
        lastBoatToCheck = boats;
        
        for (i = firstBoatToCheck; i < lastBoatToCheck; i++) {
            if (!(
                    (fleet[i]->serfs + fleet[i]->hackers == boatCapacity) || (fleet[i]->isSailing) 
			|| ((fleet[i]->serfs == halfBoatCapacity) && (fleet[i]->hackers == (halfBoatCapacity - 1))) || (fleet[i]->hackers == (halfBoatCapacity + 1))
                 )
            ) {
                fleet[i]->serfs++;
                board(MICROSOFT_EMPLOYEE, i, position);
                if (fleet[i]->serfs == boatCapacity) {
                    fleet[i]->isSailing = 1;
                    fleet[i]->serfs = 0;
                    pthread_barrier_wait(&(fleet[i]->barrier));
    				log(position, 1);
                    pthread_mutex_unlock(&mutex);
                    rowBoat(i);
                    freeWaitings();
                } else if ((fleet[i]->hackers == halfBoatCapacity)&&(fleet[i]->serfs == halfBoatCapacity)) {
                    fleet[i]->isSailing = 1;
                    fleet[i]->hackers = 0;
                    fleet[i]->serfs = 0;
                    pthread_barrier_wait(&(fleet[i]->barrier));
    				log(position, 1);
                    pthread_mutex_unlock(&mutex);
                    rowBoat(i);
                    freeWaitings();
                } else {
    				log(position, 1);
                    pthread_mutex_unlock(&mutex);
                    pthread_barrier_wait(&(fleet[i]->barrier));
                }
                
				atomic_dec_serfs();
                
                return NULL;
            }
            
            if (i == boats - 1 && firstBoatToCheck != 0) {
                lastBoatToCheck = firstBoatToCheck;
                i = -1;
            }
        }
        
		log(position, 1);
        pthread_mutex_unlock(&mutex);
        sem_wait(&serfs_queue);
    } 
    
	atomic_dec_serfs();
	
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t thr;
    int j;

    boats = (argv[1] == NULL) ? 1 : (int) (*argv[1] - '0');
    boatCapacity = (argv[2] == NULL) ? 4 : (int) (*argv[2] - '0');

    if (boatCapacity % 2 != 0) {
	printf("The boat capacity should be an even number!\n");
	return 1;
    } else {
	halfBoatCapacity = (boatCapacity / 2);
    }

    srandom(time(NULL));

    /* Initialize mutexes */
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_sail, NULL);
    pthread_mutex_init(&atomic_hackers, NULL);
    pthread_mutex_init(&atomic_serfs, NULL);
    pthread_mutex_init(&arrival, NULL);
    pthread_cond_init(&arrival_space, NULL);
    
    /* Draw initial scenario */
    clearScreen();
    drawScenario(boats);

	/* Initialize fleet */
    fleet = malloc(boats*sizeof(Boat*));
    for(j = 0; j < boats; j++){
        fleet[j] = newBoat(j);
		animateStoppedBoat(fleet[j], &mutex_sail);
    }
    
    /* Initialize people queue */
	queue = malloc(sizeof(Queue));
	queue->length = boatCapacity * boats;
	queue->queue = malloc(queue->length * sizeof(int));
    for (j = 0; j < queue->length; j++) {
		queue->queue[j] = -1;
	}
    
    /* Initialize semaphores */
    sem_init(&hackers_queue, 0, 0);
    sem_init(&serfs_queue, 0, 0);
    
    /* New people come all the time! */
    for (;;) {
        usleep(100000);
        j = random() % 2;
        if (j) {
            pthread_create(&thr, NULL, f_thread_hacker, NULL);
        } else {
            pthread_create(&thr, NULL, f_thread_serf, NULL);
        }
    }

    pthread_exit(NULL);
}
