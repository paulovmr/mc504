/*
 * Lab 01
 * Guilherme Henrique Nunes RA 117108
 * Paulo  Martins RA
 * Thiago Oliveira RA
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t hackers_queue, serfs_queue;
pthread_mutex_t mutex;
pthread_barrier_t barrier;
volatile int hackers = 0;
volatile int serfs = 0;

void board(char* type){
	printf("\nI'm boarding and i'm a %s!", type);
}

void rowBoat(){
	printf("\nROW BOAT!!");
}

void *f_thread_hacker() {
	
	int captain = 0;

	pthread_mutex_lock (&mutex);
	
	hackers++;
	
	if(hackers == 4){
		sem_post(&hackers_queue);
		sem_post(&hackers_queue);
		sem_post(&hackers_queue);
		sem_post(&hackers_queue);
		hackers = 0;
		captain = 1;
	}else
	if(hackers == 2 && serfs >= 2){
		sem_post(&hackers_queue);
		sem_post(&hackers_queue);
		sem_post(&serfs_queue);
		sem_post(&serfs_queue);
		hackers = 0;
		serfs -= 2;
		captain = 1;
	}else{
		pthread_mutex_unlock (&mutex);		
	}


	sem_wait(&hackers_queue);
	board("HACKER");	
	pthread_barrier_wait(&barrier);

	if(captain){
		rowBoat();
		pthread_mutex_unlock (&mutex);
	}
		
	return NULL;
}

void *f_thread_serf() {

	int captain = 0;

	pthread_mutex_lock (&mutex);
	
	serfs++;
	
	if(serfs == 4){
		sem_post(&serfs_queue);
		sem_post(&serfs_queue);
		sem_post(&serfs_queue);
		sem_post(&serfs_queue);
		serfs = 0;
		captain = 1;
	}else
	if(serfs == 2 && hackers >= 2){
		sem_post(&hackers_queue);
		sem_post(&hackers_queue);
		sem_post(&serfs_queue);
		sem_post(&serfs_queue);
		serfs = 0;
		hackers -= 2;
		captain = 1;
	}else{
		pthread_mutex_unlock (&mutex);		
	}

	sem_wait(&serfs_queue);
	board("SERF");
	pthread_barrier_wait(&barrier);
	
	if(captain){
		rowBoat();
		pthread_mutex_unlock (&mutex);
	}
		
	return NULL;
}

int main() {
	pthread_t thr;
	int j = 0;

	srandom(time(NULL));
	
	/* Initialize mutexes */
	pthread_mutex_init(&mutex, NULL);
	
	/* Initialize semaphores */
	sem_init(&hackers_queue, 0, 0);
	sem_init(&serfs_queue, 0, 0);
	
	/* Initialize barriers */
	pthread_barrier_init(&barrier, NULL, 4);
	
	
	for (;;){
		sleep(1);
		j = random()%2;
		if(j)
			pthread_create(&thr, NULL, f_thread_hacker, NULL);
		else
			pthread_create(&thr, NULL, f_thread_serf, NULL);
	}

	pthread_exit(NULL);
}

