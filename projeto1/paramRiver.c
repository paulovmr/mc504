/*
 * Projeto 1 - Problema da travessia do rio
 *
 * Equipe:
 *  Guilherme Henrique Nunes    RA 117108
 *  Paulo Vitor Martins do Rego RA 118343
 *  Thiago de Oliveira Pires    RA 123153
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

Boat* newBoat(int position){
    int i;
    Boat* x = malloc (sizeof *x);
    x->position = position;
    x->x = 11;
    
    i = 3 * (boatCapacity / 2);
    if (i < 6) i = 6;
    x->y = 2 + (position * i);
    
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
        usleep(500000);
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

void *f_thread_hacker() {
    
    int i, position, firstBoatToCheck, lastBoatToCheck;

    position = threadArrival(HACKER);
    atomic_inc_hackers();

    while (1) {
        pthread_mutex_lock(&mutex);
        
        firstBoatToCheck = lastBoatSailed;
        lastBoatToCheck = boats;
        
        for (i = firstBoatToCheck; i < lastBoatToCheck; i++) {
            if (!(
                     (fleet[i]->serfs + fleet[i]->hackers == boatCapacity) ||  // barco cheio
                     (fleet[i]->isSailing) || // barco atravessando
                     ((fleet[i]->hackers == halfBoatCapacity) && (fleet[i]->serfs > 0)) || // apenas servos podem entrar
                     (fleet[i]->serfs >= (halfBoatCapacity + 1)) // apenas servos podem entrar
                 )
            ) {
                fleet[i]->hackers++;
                board(HACKER, i, position);
                if (fleet[i]->hackers == boatCapacity) {
                    fleet[i]->isSailing = 1;
                    fleet[i]->hackers = 0;
                    pthread_barrier_wait(&(fleet[i]->barrier));
                    pthread_mutex_unlock(&mutex);
                    rowBoat(i);
                    freeWaitings();
                } else if ((fleet[i]->hackers == halfBoatCapacity)&&(fleet[i]->serfs == halfBoatCapacity)) {
                    fleet[i]->isSailing = 1;
                    fleet[i]->hackers = 0;
                    fleet[i]->serfs = 0;
                    pthread_barrier_wait(&(fleet[i]->barrier));
                    pthread_mutex_unlock(&mutex);
                    rowBoat(i);
                    freeWaitings();
                } else {
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
        
        pthread_mutex_unlock(&mutex);
        sem_wait(&hackers_queue);
    }  

    atomic_dec_hackers();
    
    return NULL;
}

void *f_thread_serf() {

    int i, position, firstBoatToCheck, lastBoatToCheck;

    position = threadArrival(SERF);
    atomic_inc_serfs();

    while (1) {
        pthread_mutex_lock(&mutex);
        
        firstBoatToCheck = lastBoatSailed;
        lastBoatToCheck = boats;
        
        for (i = firstBoatToCheck; i < lastBoatToCheck; i++) {
            if (!(
                     (fleet[i]->serfs + fleet[i]->hackers == boatCapacity) || // barco cheio
                     (fleet[i]->isSailing) || // barco atravessando
                     ((fleet[i]->serfs == halfBoatCapacity) && (fleet[i]->hackers > 0)) || // apenas hackers podem entrar
                     (fleet[i]->hackers >= (halfBoatCapacity + 1)) // apenas hackers podem entrar
                 )
            ) {
                fleet[i]->serfs++;
                board(SERF, i, position);
                if (fleet[i]->serfs == boatCapacity) {
                    fleet[i]->isSailing = 1;
                    fleet[i]->serfs = 0;
                    pthread_barrier_wait(&(fleet[i]->barrier));
                    pthread_mutex_unlock(&mutex);
                    rowBoat(i);
                    freeWaitings();
                } else if ((fleet[i]->hackers == halfBoatCapacity)&&(fleet[i]->serfs == halfBoatCapacity)) {
                    fleet[i]->isSailing = 1;
                    fleet[i]->hackers = 0;
                    fleet[i]->serfs = 0;
                    pthread_barrier_wait(&(fleet[i]->barrier));
                    pthread_mutex_unlock(&mutex);
                    rowBoat(i);
                    freeWaitings();
                } else {
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
        
        pthread_mutex_unlock(&mutex);
        sem_wait(&serfs_queue);
    } 
    
    atomic_dec_serfs();
    
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t thr;
    int j;

    boats = 1;
    boatCapacity = 4;
    if (argv[1] != NULL) {
        boats = atoi(argv[1]);
        
        if (argv[2] != NULL) {
            boatCapacity = atoi(argv[2]);
        }
    }

    if (boatCapacity % 2 != 0) {
        printf("A capacidade do barco deve ser um número par!\n");
        return 1;
    } else {
        halfBoatCapacity = (boatCapacity / 2);
    }

    srandom(time(NULL));

    /* Inicializa mutexes */
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_sail, NULL);
    pthread_mutex_init(&atomic_hackers, NULL);
    pthread_mutex_init(&atomic_serfs, NULL);
    pthread_mutex_init(&arrival, NULL);
    pthread_cond_init(&arrival_space, NULL);
    
    /* Desenha cenário inicial */
    clearScreen();
    drawScenario(boats, boatCapacity);

    /* Inicializa frota */
    fleet = malloc(boats*sizeof(Boat*));
    for(j = 0; j < boats; j++){
        fleet[j] = newBoat(j);
        animateStoppedBoat(fleet[j], &mutex_sail);
    }
    
    /* Inicializa fila de pessoas */
    queue = malloc(sizeof(Queue));
    j = boatCapacity;
    if (j < 4) j = 4;
    queue->length = j * boats;
    queue->queue = malloc(queue->length * sizeof(int));
    for (j = 0; j < queue->length; j++) {
        queue->queue[j] = -1;
    }
    
    /* Inicializa semáforos */
    sem_init(&hackers_queue, 0, 0);
    sem_init(&serfs_queue, 0, 0);
    
    /* Novas pessoas querem embarcar a todo o tempo! */
    for (;;) {
        sleep(1);
        j = random() % 2;
        if (j) {
            pthread_create(&thr, NULL, f_thread_hacker, NULL);
        } else {
            pthread_create(&thr, NULL, f_thread_serf, NULL);
        }
    }

    pthread_exit(NULL);
}
