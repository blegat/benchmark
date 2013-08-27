#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <err.h>
#include <unistd.h>

#include "benchmark.h"

#define NTHREAD 8192
#define NLENGTH 50000

typedef struct scanargs {
	int start;
	int stop;
	int* array;
} scanargs;

typedef struct result {
	int count;
	int index;
} result;

int primeFactors(int n) {
	int i,count=0;
	for(i=2;i<=n;i++)
	{
		if(n%i==0)
		{
			count++;
			n=n/i;
			i--;
			if(n==1)
				break;
		} 
	} 
	return count;
}

void* scan(void* param) {
	scanargs* args = (scanargs*)param;
	result *res = (result*)malloc(sizeof(result));
	res->count = 0;
	res->index = 0;
	
	int i,p;
	for (i = args->start; i<=args->stop; i++) {
		p = primeFactors(args->array[i]);
		//printf("number: %d ; primes: %d ; index: %d\n", args->array[i],p,i);
		if (p>res->count) {
			res->count = p;
			res->index = i;
		}
	}
	pthread_exit((void*)res);
}

int main (int argc, char* argv[]) {
	
	timer *t = timer_alloc();
	recorder *thread_rec = recorder_alloc("thread.csv");
	
	srand (1337);
	int* array = (int*)malloc(sizeof(int)*NLENGTH);
	if (array == NULL) err(1,"erreur malloc");
	int i;
	for (i=0; i<NLENGTH; i++) {
		array[i] = rand() % NLENGTH;
	}
	for (i = 1; i<=NTHREAD; i=i*2) {
		start_timer(t);
		int error = 0;
		
		pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t)*i);
		if (threads == NULL) err(1,"erreur malloc");
		scanargs** args = (scanargs**)malloc(sizeof(scanargs*)*i);
		if (args == NULL) err(1,"erreur malloc");
		result* res[i];
		
		int j;
		for (j=0; j<i; j++) {
			args[j] = (scanargs*)malloc(sizeof(scanargs));
			args[j]->start = j*(NLENGTH/i);
			args[j]->stop = (j+1)*(NLENGTH/i)-1;
			args[j]->array = array;
			error = pthread_create(&threads[j],NULL,&scan,(void*)args[j]);
			if(error!=0) err(error,"erreur create");
		}
		
		for (j=0; j<i; j++) {
			pthread_join(threads[j],(void**)&res[j]);
		}
		
		int index = -1;
		int max = 0;
		for (j=0; j<i; j++) {
			if (max<res[j]->count) {
				max = res[j]->count;
				index = res[j]->index;
				free(res[j]);
				free(args[j]);
			}
		}
		
		printf("%d\n",index);
		int it = i;
		int numCPU = sysconf( _SC_NPROCESSORS_ONLN );
		if (it > numCPU ) it = numCPU;
		write_record_n(thread_rec,i,stop_timer(t)/it,NTHREAD);
		
		free(threads);
		free(args);
	}
	
	free(array);
}