#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <pthread.h>
#include <err.h>
#include <unistd.h>

#include "benchmark.h"

#define ARRAY_LEN 50000
#define KEY 1252
#define DO 20

void* work (void* param) {
	int* array = (int*) param;
	int size = array[0];
	int i;
	int tot;
	for (i = 1; i<size; i++) {
		tot += array[i];
	}
	array[size-1] = tot;
	//printf("%d\n",tot);
	return NULL;
}

int main (int argc, char *argv[])  {
	timer *t = timer_alloc();
	recorder *heap_rec = recorder_alloc("heap.csv");
	recorder *shm_rec = recorder_alloc("shm.csv");
	pthread_t thread;
	
	int i;
	for (i=100; i<=ARRAY_LEN; i+=100) {
		start_timer(t);
		int* array = (int*)malloc(sizeof(int)*i);
		
		int k;
		for (k=0; k<DO; k++) {
		
			int j;
			for (j=0; j<i; j++) {
				array[j] = j+1;
			}
			array[0] = i;
			pthread_create(&thread,NULL,work,(void*)array);
			pthread_join(thread,NULL);
		}
		write_record_n(heap_rec,i,stop_timer(t),ARRAY_LEN);
		free(array);
	}
	
	for (i=100; i<=ARRAY_LEN; i+=100) {
		
		int shm_id = shmget(KEY, sizeof(int)*i, IPC_CREAT | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
		if (shm_id < 0) err(-1, "erreur lors de shmget");
		int* array = shmat(shm_id,NULL,0);
		//if (array = -1) err(-1, "erreur lors de shmat");
		
		start_timer(t);
		
		int k;
		for (k=0; k<DO; k++) {
		
			int j;
			for (j=0; j<i; j++) {
				array[j] = j+1;
			}
			array[0] = i;
		
			pid_t pid = fork();
		
			if (pid < 0) {
				err(pid,"erreur de fork");
			} else if (pid == 0) {
				/*
				int shm_id = shmget(KEY, sizeof(int)*i, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
				if (shm_id < 0) err(-1, "erreur lors de shmget dans le fils");
				int* array = shmat(shm_id,NULL,0);
				//if (array = (void*)-1) err(-1, "erreur lors de shmat dans le fils");
				 */
				int size = array[0];
				int i;
				int tot;
				for (i = 1; i<size; i++) {
					tot += array[i];
				}
				array[size-1] = tot;
				//printf("%d\n",tot);
				int dt = shmdt(array);
				if (dt < 0) err(dt,"erreur lors de shmdt dans le fils");
				exit(0);
			} else {
				waitpid(pid,NULL,0);
			}
		}
		write_record_n(shm_rec,i,stop_timer(t),ARRAY_LEN);
		int ctl = shmctl(shm_id, IPC_RMID, NULL);
		if (ctl < 0) err(ctl,"erreur lors de shmctl");
		int dt = shmdt(array);
		if (dt < 0) err(dt,"erreur lors de shmdt");
		shmctl(shm_id, IPC_RMID, NULL);
				
	}

	recorder_free(heap_rec);
	recorder_free(shm_rec);
	timer_free(t);

	return EXIT_SUCCESS;
}
