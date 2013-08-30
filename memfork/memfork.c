/**
	\file memfork.c
	\brief Ce programme compare le temps nécessaire pour effectuer le "copy-on-write"

	Pour cela, nous allons donc faire un fork, modifier deux fois les données dans le fils et comparer la différence entre les deux.
	Cette différence représente le temps nécessaire au copy-on-write.

	Dans le cas de l'utilisation de perf, on ne réécrit pas dans les records et n'effectue une seule des deux modifications. Dans ce cas la, on fait le perf sur la taille maximun
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>

#include "../lib/benchmark.c"
#include "../lib/benchmark.h"

#define N 10000
#define MULTIPLICATEUR 1024

timer *t;
recorder* bftfork_rec;
recorder* aftfork_rec;
recorder* aftmodif_rec;

/**
	Parcours un tableau et calcul le temps nécessaire
*/
long int parcoursTab(timer* t, int i, char** tab) {

	int j, k, res;
	start_timer(t);
	for(j=0; j<i; j++)
		for(k=0; k<i; k++) {
			res +=tab[j][k];
			tab[j][k]=j+k;
		}
	return stop_timer(t);
}


int main (int argc, char *argv[])  {
	// Regarde les arguments
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
	int status, i, j;
	long int resultbft, resultaft;
	for (i = MULTIPLICATEUR; i < N; i+=MULTIPLICATEUR) {

		if(perfaft || perfbft)
			i = N;

		char ** tab = malloc(i*sizeof(char*));
		if(tab == NULL) {
			perror("malloc fail");
			exit(EXIT_FAILURE);
		}
		for(j=0; j<i; j++)
			tab[j] = malloc(i*sizeof(char));


		// On le parcours une fois avant le fork pour avoir une mesure de référence
		if( !perfbft && !perfaft)
			write_record(bftfork_rec, i/MULTIPLICATEUR, parcoursTab(t, i, tab));


		pid = fork();

		if (pid == -1) {
			// erreur à l'exécution de fork
			perror("fork");
			return EXIT_FAILURE;
		}
		if (pid == 0) {
			// processus fils
			sleep(1);

			// On le parcours lors du copy-on-write
			if(!perfaft)
				resultbft = parcoursTab(t, i, tab);

			sleep(1);

			// On le parcours apres le copy-on-write
			if(! perfbft)
				resultaft = parcoursTab(t, i, tab);


			if(!perfbft && !perfaft) {
				write_record(aftfork_rec, i/MULTIPLICATEUR, resultbft);
				write_record(aftmodif_rec, i/MULTIPLICATEUR, resultaft);
			}

			// Libération du tableau et des records si ils sont alloués
			if(!perfbft && !perfaft) {
			      	recorder_free(bftfork_rec);
			  	recorder_free(aftfork_rec);
				recorder_free(aftmodif_rec);
				timer_free(t);
			}

			free(tab);
			tab=NULL;
			for(j=0; j<i; j++) {
				free(tab[j]);
				tab[j]=NULL;
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
		
		
	}

	// Libération du tableau et des records si ils sont alloués
	if(!perfbft && !perfaft) {
		recorder_free(bftfork_rec);
	  	recorder_free(aftfork_rec);
		recorder_free(aftmodif_rec);
		timer_free(t);
	}


  return EXIT_SUCCESS;
}
