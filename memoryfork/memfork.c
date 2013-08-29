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


int main (int argc, char *argv[])  {
	int perfbef = argc>1 && strncmp(argv[1], "--before", 9);
	int perfaft = argc>1 &&strncmp(argv[1], "--after", 8);


	// Déclare un timer, ainsi que deux recorder qui vont contenir les résultats de l'exécution du programme
	timer *t = timer_alloc();
	recorder* bftf_rec = recorder_alloc("memfor-beforefork.csv");
 	recorder* aftf_rec = recorder_alloc("memfor-aftfork.csv");
	recorder* aftm_rec = recorder_alloc("memfor-aftmodif.csv");
	
	pid_t pid;
	int status, i,j,k,res;
	for (i = MULTIPLICATEUR; i < N; i+=MULTIPLICATEUR) {

		if(perfligne || perfcolonne)
			i = N;

		char ** tab = malloc(i*sizeof(char*));
		for(j=0; j<i; j++) 
			tab[j] = malloc(i*sizeof(char));

		if( ! (perfbef || perfaft)){
			start_timer(t);
			for(j=0; i<i; j++) 
				for(k=0; k<i; k++) {
					res +=tab[j][k];
					tab[j][k]=j+k;
			}
			write_record(bftf_rec, i/MULTIPLICATEUR, stop_timer(t));	
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
				for(j=0; i<i; j++) 
					for(k=0; k<i; k++) {
						res +=tab[j][k];
						tab[j][k]=j+k;
					}	
				write_record(aftf_rec, i/MULTIPLICATEUR, stop_timer(t));	
			}

			sleep(1);
			
			if(! perfbef) {
				start_timer(t);
				for(j=0; i<i; j++) 
					for(k=0; k<i; k++) {
						res +=tab[j][k];
						tab[j][k]=j+k;
				}
				write_record(aftm_rec, i/MULTIPLICATEUR, stop_timer(t));	
			}

		      	recorder_free(bftf_rec);
		  	recorder_free(aftf_rec);
			recorder_free(aftm_rec);	
			timer_free(t);
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
	
	recorder_free(bftf_rec);
  	recorder_free(aftf_rec);
	recorder_free(aftm_rec);
	timer_free(t);

  return EXIT_SUCCESS;
}
