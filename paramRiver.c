/*
 * Lab 01
 * Guilherme Henrique Nunes RA 117108
 * Paulo    Martins RA
 * Thiago Oliveira RA
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct boat{
    volatile int hackers;
    volatile int serfs;
    volatile int isSailing;
    pthread_barrier_t barrier;
};

typedef struct boat *Boat;

sem_t hackers_queue, serfs_queue;
pthread_mutex_t mutex;
pthread_mutex_t mutex_sail;
//pthread_barrier_t barrier;
volatile int hackers = 0;
volatile int serfs = 0;
volatile int boats = 1;
volatile Boat* fleet;

Boat newBoat(){
                Boat x = malloc (sizeof *x);
                x->hackers = 0;
                x->serfs = 0;
                x->isSailing = 0;
                pthread_barrier_init(&(x->barrier), NULL, 4);
                return x;
}

void threadArrival(int i){
    if(i == 0)
        printf("\nArrive - Hackers: [%d]", hackers);
    else
        printf("\nArrive - Serfs: [%d]", serfs);
}

void board(char* type, int i){
    printf("\nI'm boarding the boat [%d] and i'm a %s!", i, type);
}

void rowBoat(int i){
    pthread_mutex_lock(&mutex_sail);
    printf("\nBOAT [%d] ---------------------------------------------------------->>>>>>", i);
    sleep(10);
    printf("\nBOAT [%d] <<<<<<----------------------------------------------------------", i);
    pthread_mutex_unlock(&mutex_sail);
    fleet[i]->isSailing = 0;
}

void freeWaitings(){
    int j;

    for(j = 0; j < serfs; j++) sem_post(&serfs_queue);
    for(j = 0; j < hackers; j++) sem_post(&hackers_queue);
}

void *f_thread_hacker() {
    
    int i;

    hackers++;
    threadArrival(0);

    pthread_mutex_lock (&mutex);

    while(1){    
        for(i = 0; i < boats && (!fleet[i]->isSailing); i++){
            if( !(
                    ((fleet[i]->hackers == 2) && (fleet[i]->serfs == 1)) || (fleet[i]->serfs == 3)
                 )
            ){
                fleet[i]->hackers++;
                board("HACKER", i);
                if( fleet[i]->hackers == 4){
                    fleet[i]->isSailing = 1;
                    fleet[i]->hackers = 0;
                    pthread_barrier_wait(&(fleet[i]->barrier));
                    pthread_mutex_unlock(&mutex);
                    rowBoat(i);
                    freeWaitings();
                }else if((fleet[i]->hackers == 2)&&(fleet[i]->serfs == 2)){
                    fleet[i]->isSailing = 1;
                    fleet[i]->hackers = 0;
                    fleet[i]->serfs = 0;
                    pthread_barrier_wait(&(fleet[i]->barrier));
                    pthread_mutex_unlock(&mutex);
                    rowBoat(i);
                    freeWaitings();
                }else{
                    pthread_mutex_unlock(&mutex);
                    pthread_barrier_wait(&(fleet[i]->barrier));
                }
                hackers--;
                return NULL;
            }
        }
        pthread_mutex_unlock(&mutex);
        sem_wait(&hackers_queue);
    }  

    hackers--;
    return NULL;
}

void *f_thread_serf() {

    int i;

    serfs++;
    threadArrival(1);

    pthread_mutex_lock (&mutex);

    while(1){    
        for(i = 0; i < boats && (!fleet[i]->isSailing); i++){
            if( !(
                    ((fleet[i]->serfs == 2) && (fleet[i]->hackers == 1)) || (fleet[i]->hackers == 3)
                 )
            ){
                fleet[i]->serfs++;
                board("SERF", i);
                if( fleet[i]->serfs == 4){
                    fleet[i]->isSailing = 1;
                    fleet[i]->serfs = 0;
                    pthread_barrier_wait(&(fleet[i]->barrier));
                    pthread_mutex_unlock(&mutex);
                    rowBoat(i);
                    freeWaitings();
                }else if((fleet[i]->hackers == 2)&&(fleet[i]->serfs == 2)){
                    fleet[i]->isSailing = 1;
                    fleet[i]->hackers = 0;
                    fleet[i]->serfs = 0;
                    pthread_barrier_wait(&(fleet[i]->barrier));
                    pthread_mutex_unlock(&mutex);
                    rowBoat(i);
                    freeWaitings();
                }else{
                    pthread_mutex_unlock(&mutex);
                    pthread_barrier_wait(&(fleet[i]->barrier));
                }
                serfs--;
                return NULL;
            }
        }
        pthread_mutex_unlock(&mutex);
        sem_wait(&serfs_queue);
    } 
    serfs--; 
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t thr;
    int j = 0;

    srandom(time(NULL));

    boats = (int) (*argv[1] - '0');
    fleet = malloc(boats*sizeof(Boat));

    for(j = 0; j < boats; j++){
        fleet[j] = newBoat();
    }

    /* Initialize mutexes */
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_sail, NULL);
    
    /* Initialize semaphores */
    sem_init(&hackers_queue, 0, 0);
    sem_init(&serfs_queue, 0, 0);
    
    /* Initialize barriers */
    //pthread_barrier_init(&barrier, NULL, 4);
    
    
    for (;;){
        sleep(2);
        j = random()%2;
        if(j)
            pthread_create(&thr, NULL, f_thread_hacker, NULL);
        else
            pthread_create(&thr, NULL, f_thread_serf, NULL);
    }

    pthread_exit(NULL);
}
