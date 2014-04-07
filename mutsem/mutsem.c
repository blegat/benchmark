/**
	\file mutsem.c
	\brief Ce programme compare le temps nécessaire pour lock/unlock avec un mutex et celui pour wait/post avec une sémaphore

	Pour cela, nous allons créer une chaîne de thread ayant chacun un mutex et une sémaphore "personnelle". Pour un nombre "N" donné, nous allons lancer "N" thread où chaque thread bloquera son mutex et sa sémaphore. Chaque thread attendra ensuite que le thread précédent ait débloqué son mutex/sémaphore, pour à son tour débloqué le sien. Parmis ces threads, il y a un thread spécial : le **first**.  Celui ci est chargé de débloqué en premier son mutex/sémaphore et de calculé le temps que cela prend pour lui revenir.

	En schéma :

	+-------+	+-------+			+-------+	+-------+
	| first	|	| oth 1	|	  ...		| oth 2	| 	| oth N	| 
	+-------+	+-------+			+-------+	+-------+
	  |   ^___________|  ^____________|  ^____________|  ^____________|   ^
	  |	   (1)		   (2)		   	  (3)		      |
	  |						      		      |
	  |___________________________________________________________________|
				   (1)         			


	(1) : first unlock son mut1, ce qui permet à oth N de lock son mut2 (puisqu'ils pointent vers le même mutex)
	(2) : Puisque oth 1 a pu locker son mut2, il unlock son mut1
	(3) : La chaîne continue
	(4) : Lorsque first peut enfin faire un lock sur son mut2, il arrete le timer et écrit dans le record

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>	
#include <pthread.h>
#include <unistd.h>

#include "benchmark.h"

#define MAX_SIZE 3
#define TURN 100000000

//Time by default (4)

timer *t;
int i;
recorder *sem_rec;
recorder *mut_rec;

#define MULTIPLICATEUR 1000
#define NUMBER_THREAD 10000

/**
	\brief Structure utilisé pour stocker deux mutex et deux sémaphores

	Cette structure est utilisée dans le cadre des tests comparant le temps de lock/unlock contre celui de wait/post
*/
struct arg { 
	pthread_mutex_t * mut1;
	pthread_mutex_t * mut2;
	sem_t * sem1;
	sem_t * sem2;
};

/**
	\brief Fonction threader permettant de lancer la chaine des lock/unlock puis des wait/post.

	Cette fonction lance donc la chaîne et calcul le temps qu'il faut pour la parcourir completement.
	Une fois les temps calculé, ils sont stocké dans les records
*/
void * first(void* args) {
	struct arg* mutex = (struct arg*) args;

	// Bloque sont propre mutex et sémaphore
	pthread_mutex_lock(mutex->mut1);
	sem_wait(mutex->sem1);

	// Attend une seconde pour être sur que tout le monde ait bloqué son mutex et sa sémaphore
	sleep(1);

	// Commence le timer et débloque son mutex pour commencer la cascade de lock/unlock
	start_timer(t);
	pthread_mutex_unlock(mutex->mut1);
	pthread_mutex_lock(mutex->mut2);
	write_record_n(mut_rec, i/MULTIPLICATEUR, stop_timer(t), MULTIPLICATEUR);

	sleep(1);

	// Commence le timer et débloque sa sémaphore pour commencer la cascade de wait/post
	start_timer(t);
	sem_post(mutex->sem1);
	sem_wait(mutex->sem2);
	write_record_n(sem_rec, i/MULTIPLICATEUR, stop_timer(t), MULTIPLICATEUR);

	return NULL;
}

void * other(void* args) {
	struct arg * mutex = (struct arg*) args;

	// Bloque leur propre mutex/sem
	pthread_mutex_lock(mutex->mut1);
	sem_wait(mutex->sem1);

	// Attend une seconde pour être sur que tout le monde ait bloqué son mutex et sa sémaphore	
	sleep(1);

	// Une fois que le mutex précédent est débloqué, on débloque son mutex pour permettre au suivant de faire de même
	pthread_mutex_lock(mutex->mut2);
	pthread_mutex_unlock(mutex->mut1);

	// Idem avec les sémaphores
	sem_wait(mutex->sem2);
	sem_post(mutex->sem1);

	sleep(1);

	return NULL;
}
	

int main (int argc, char *argv[])  {
	// Déclare un timer, ainsi que deux recorder qui vont contenir les résultats de l'exécution du programme
	t = timer_alloc();
	sem_rec = recorder_alloc("mutsem-sem.csv");
 	mut_rec = recorder_alloc("mutsem-mut.csv");

	int j;
	pthread_t * threads;
	pthread_mutex_t * mutex;
	sem_t * sems;
	struct arg * args;
	
	for(i=MULTIPLICATEUR; i<NUMBER_THREAD; i+=MULTIPLICATEUR) {
		/* Alloue l'espace mémoire utilisé pour stocker les threads, 
								les mutex, 
								les sémaphores et 
								les structures contenant les arguments */
		threads = malloc(i*sizeof(pthread_t));
		mutex = malloc(i*sizeof(pthread_mutex_t));
		sems = malloc(i*sizeof(sem_t));
		args = malloc(i*sizeof(struct arg));

		if(threads == NULL || mutex == NULL || sems == NULL || args == NULL) {
			perror("Impossible d'allouer la mémoire nécessaire : mutsem.c");
   			exit(EXIT_FAILURE);
		}

		// Initialisation des mutex et sémaphores
		for(j=0; j<i;j++) {
			pthread_mutex_init(mutex+j, NULL);
			sem_init(sems+j,0,1);
		}

		/** Remplissage des N arguments compris entre 1 et N.
		    Pour un argument A tel que : * 1 < A < N-1 : mut1 de A = mut2 de A-1 et mut2 de A = mut1 de A+1
						 * A = 1       : mut1 de A = mut2 de N et mut2 de A = mut1 de A+1
						 * A = N       : mut1 de A = mut2 de A-1 et mut2 de A = mut1 de 1
		*/
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
		
		// Démarrage des threads
		pthread_create(threads, NULL, first, (void*)args);
		for(j=1; j<i;j++)
			pthread_create(threads+j, NULL, other, (void*)(args+j));
		
		// Récupération des threads
		for(j=i-1; j>=0;j--)
			pthread_join(*(threads+j), NULL);
	
		// Destruction des mutex et des sémaphores
		for(j=0; j<i; j++) {
			pthread_mutex_destroy(mutex+j);
			sem_destroy(sems+j);
		}

		// Libération de la mémoire
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
