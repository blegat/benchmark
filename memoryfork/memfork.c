#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>

#include "benchmark.h"

#define N 10000
#define MULTIPLICATEUR 1024

timer *t;
recorder* bftfork_rec;
recorder* aftfork_rec;
recorder* aftmodif_rec;

int main (int argc, char *argv[])  {
	int perfbft = argc>1 && strncmp(argv[1], "--before", 9);
	int perfaft = argc>1 &&strncmp(argv[1], "--after", 8);


	if((!perfbft) && !perfaft) {
		// Déclare un timer, ainsi que deux recorder qui vont contenir les résultats de l'exécution du programme
		t = timer_alloc();
		bftfork_rec = recorder_alloc("memfor-beforefork.csv");
	 	aftfork_rec = recorder_alloc("memfor-aftfork.csv");
		aftmodif_rec = recorder_alloc("memfor-aftmodif.csv");
	}

	pid_t pid;
	int status, i,j,k,res;
	long int resultbft, resultaft;
	for (i = MULTIPLICATEUR; i < N; i+=MULTIPLICATEUR) {

		if(perfaft || perfbft)
			i = N;

		char ** tab = malloc(i*sizeof(char*));
		for(j=0; j<i; j++)
			tab[j] = malloc(i*sizeof(char));

		if( !perfbft && !perfaft){
			start_timer(t);
			for(j=0; j<i; j++) 
				for(k=0; k<i; k++) {
					res +=tab[j][k];
					tab[j][k]=j+k;
			}
			write_record(bftfork_rec, i/MULTIPLICATEUR, stop_timer(t));	
		}


		pid = fork();

		if (pid == -1) {
			// erreur à l'exécution de fork
			perror("fork");
			return EXIT_FAILURE;
		}
		// pas d'erreur
		// BEGIN
		if (pid == 0) {
			// processus fils
			sleep(1);

			k=1;
			j=1;

			if(!perfaft) {
			      	start_timer(t);
				for(j=0; j<i; j++) 
					for(k=0; k<i; k++) {
						res +=tab[j][k];
						tab[j][k]=j+k;
					}
				resultbft = stop_timer(t);
			}

			sleep(1);
			
			if(! perfbft) {
				start_timer(t);
				for(j=0; j<i; j++) 
					for(k=0; k<i; k++) {
						res +=tab[j][k];
						tab[j][k]=j+k;
				}
				resultaft = stop_timer(t);	
			}

			if(!perfbft && !perfaft) {
				write_record(aftfork_rec, i/MULTIPLICATEUR, resultbft);
				write_record(aftmodif_rec, i/MULTIPLICATEUR, resultaft);
			}
			
			if(!perfbft && !perfaft) {
			      	recorder_free(bftfork_rec);
			  	recorder_free(aftfork_rec);
				recorder_free(aftmodif_rec);
				timer_free(t);
			}
		      	return EXIT_SUCCESS;
		}
		else {
		     	// processus père
		      	pid = waitpid(pid, &status, 0);
		      	if (pid == -1) {
				perror("wait");
				return EXIT_FAILURE;
		      	}
		}
	    	// END
	}
	if(!perfbft && !perfaft) {
		recorder_free(bftfork_rec);
	  	recorder_free(aftfork_rec);
		recorder_free(aftmodif_rec);
		timer_free(t);
	}

  return EXIT_SUCCESS;
}
