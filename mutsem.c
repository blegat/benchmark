#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>	
#include <pthread.h>

#include "benchmark.h"

#define MAX_SIZE 3
#define TURN 100000000

//Time by default (4)

timer *t;
int i;
recorder *sem_rec;
recorder *mut_rec;
#define NUMBER_THREAD 10000

struct arg { 
	pthread_mutex_t * mut1;
	pthread_mutex_t * mut2;
	sem_t * sem1;
	sem_t * sem2;
};

void * first(void* args) {
	struct arg* mutex = (struct arg*) args;

	pthread_mutex_lock(mutex->mut1);
	sem_wait(mutex->sem1);

	sleep(1);

	start_timer(t);
	pthread_mutex_unlock(mutex->mut1);
	pthread_mutex_lock(mutex->mut2);
	write_record(mut_rec, i/1000, stop_timer(t)/1000);

	sleep(1);

	start_timer(t);
	sem_post(mutex->sem1);
	sem_wait(mutex->sem2);
	write_record(sem_rec, i/1000, stop_timer(t)/1000);

	return NULL;
}

void * other(void* args) {
	struct arg * mutex = (struct arg*) args;

	// Bloque leur propre mutex/sem
	pthread_mutex_lock(mutex->mut1);
	sem_wait(mutex->sem1);

	sleep(1);

	pthread_mutex_lock(mutex->mut2);
	pthread_mutex_unlock(mutex->mut1);

	sem_wait(mutex->sem2);
	sem_post(mutex->sem1);

	sleep(1);

	return NULL;
}
	

int main (int argc, char *argv[])  {
	t = timer_alloc();
	sem_rec = recorder_alloc("mutsem-sem.csv");
 	mut_rec = recorder_alloc("mutsem-mut.csv");

	int j;
	pthread_t * threads;
	pthread_mutex_t * mutex;
	sem_t * sems;
	struct arg * args;
	
	for(i=1000; i<NUMBER_THREAD; i+=1000) {
		threads = malloc(i*sizeof(pthread_t));
		mutex = malloc(i*sizeof(pthread_mutex_t));
		sems = malloc(i*sizeof(sem_t));
		args = malloc(i*sizeof(struct arg));

		for(j=0; j<i;j++) {
			pthread_mutex_init(mutex+j, NULL);
			sem_init(sems+j,0,1);
		}

		//Remplissage des arguments
		for(j=0; j<i-1;j++) {
			(args+j)->mut1 = mutex+j;
			(args+j)->mut2 = (mutex+j+1);
			(args+j)->sem1 = sems+j;
			(args+j)->sem2 = (sems+j+1);
		}
		(args+i-1)->mut1 = (mutex+i-1);
		(args+i-1)->mut2 = mutex;
		(args+i-1)->sem1 = (sems+i-1);
		(args+i-1)->sem2 = sems;
		

		//Démarrage des threads
		pthread_create(threads, NULL, first, (void*)args);
		for(j=1; j<i;j++){
			pthread_create(threads+j, NULL, other, (void*)(args+j));
		}
		

		for(j=i-1; j>=0;j--){
			pthread_join(*(threads+j), NULL);
		}
	
		for(j=0; j<i; j++) {
			pthread_mutex_destroy(mutex+j);
			sem_destroy(sems+j);
		}


		free(threads);
		threads=NULL;
		free(mutex);
		mutex =NULL;
		free(sems);
		sems=NULL;	
		free(args);
		args = NULL;
	}

	recorder_free(sem_rec);
  	recorder_free(mut_rec);
	timer_free(t);

  return EXIT_SUCCESS;
}
