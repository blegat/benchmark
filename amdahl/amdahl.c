#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <err.h>
#include <unistd.h>
#include <sys/wait.h>

#include "benchmark.h"

#define NTHREAD 16
#define NLENGTH 32000

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
	recorder *proc_rec = recorder_alloc("proc.csv");
	
	srand (1337);
	int* array = (int*)malloc(sizeof(int)*NLENGTH);
	if (array == NULL) err(1,"erreur malloc");
	int i;
	for (i=0; i<NLENGTH; i++) {
		array[i] = rand() % NLENGTH;
	}
	for (i = 1; i<=NTHREAD; i=i+1) {
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
			error=pthread_join(threads[j],(void**)&res[j]);
			if(error!=0) err(error,"erreur create");
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
	
	puts("proc");
	// PROCESS
	
	for (i = 1; i<=NTHREAD; i=i+1) {
		
		start_timer(t);
		int error = 0;
		pid_t pid[i];
		int fd[i][2];
		
		int j;
		for (j=0; j<i; j++) {
			int start = j*(NLENGTH/i);
			int stop = (j+1)*(NLENGTH/i)-1;
			pipe(fd[j]);
			pid[j] = fork();
			
			if (pid[j] == 0) {
				
				int k,p,index,count=0;
				for (k = start; k<=stop; k++) {
					p = primeFactors(array[k]);
					//printf("number: %d ; primes: %d ; index: %d\n", args->array[i],p,i);
					if (p>count) {
						count = p;
						index = k;
					}
				}
				
				//printf("index: %d count: %d\n",index,count);
				
				index = index*1000;
				index += count;
				//printf("index: %d\n",index);
				
				close(fd[j][0]);
				write(fd[j][1], &index, sizeof(index));
				close(fd[j][1]);
				
				exit(0);
			} else if (pid[j] < 0) {
				err(-1,"erreur de fork");
			}
		}
		int max = 0,index,ret;
		for (j=0; j<i; j++) {
			
			close(fd[j][1]);
			read(fd[j][0], &ret, sizeof(ret));
			close(fd[j][0]);
			
			pid[j] = waitpid(pid[j],NULL,0);
			if(pid[j]<1)err(-1,"erreur waitpid");
			//printf("ret: %d\n",ret);
			if (max < ret%1000) {
				max = ret%1000;
				index = ret/1000;
			}
		}
		
		printf("%d\n",index);
		int it = i;
		int numCPU = sysconf( _SC_NPROCESSORS_ONLN );
		if (it > numCPU ) it = numCPU;
		write_record_n(proc_rec,i,stop_timer(t),NTHREAD);
	}
	
	recorder_free(thread_rec);
	recorder_free(proc_rec);
	timer_free(t);
	free(array);
}