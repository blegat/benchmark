#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>	
#include <pthread.h>

#include "benchmark.h"

#define MAX_SIZE 4000
#define MULTIPLICATEUR 100
	

/**
	On parcourt le double tableau par ligne, en écrivant les performances dans le record
*/
void ligne(int i, recorder* lig_rec, timer *t, int ** tab) {
	int m,n,res=0;
	start_timer(t);
	for(m=0; m<i; m++) 
		for(n=0; n<i; n++) 	
			res+= tab[n][m];
		
	write_record_n(lig_rec, i/MULTIPLICATEUR, stop_timer(t), MULTIPLICATEUR);
}

/**
	On parcourt le double tableau par colonne, en écrivant les performances dans le record
*/
void colonne(int i, recorder* col_rec, timer *t, int ** tab) {
	int m,n,res=0;
	start_timer(t);
	for(m=0; m<i; m++) 
		for(n=0; n<i; n++) 	
			res+= tab[m][n];
		
	write_record_n(col_rec, i/MULTIPLICATEUR, stop_timer(t), MULTIPLICATEUR);
}

/**
	\brief Compare les performances lorsque l'on parcourt un tableau par ligne ou par colonne

	Ce programme additionne les valeurs d'un tableau en le parcourant :
		* Soit par colonne
		* Soit par ligne

	Note : Le plus rapide profite de la localité spatiale
*/
int main (int argc, char *argv[])  {

  	int perfligne = argc>1 && strncmp(argv[1], "--ligne", 8);
	int perfcolonne = argc>1 &&strncmp(argv[1], "--colonne", 10);

	// Déclare un timer, ainsi que deux recorder qui vont contenir les résultats de l'exécution du programme
	timer * t = timer_alloc();
	recorder * lig_rec = recorder_alloc("tab-lig.csv");
 	recorder * col_rec = recorder_alloc("tab-col.csv");
		
	int i,j;
	for(i=MULTIPLICATEUR; i<MAX_SIZE ; i+=MULTIPLICATEUR) {
	
		if(perfligne || perfcolonne)
			i = MAX_SIZE;

		// On crée le tableau
		int ** tab = malloc(i*sizeof(int*));
		if(tab == NULL) {
			perror("malloc fail");
			exit(EXIT_FAILURE);
		}
		for(j=0; j<i; j++) 
			tab[j] = malloc(i*sizeof(int));

		if(!perfcolonne)
			ligne(i, lig_rec, t, tab);
		if(!perfligne)
			colonne(i, col_rec, t, tab);

		//On libère la mémoire
		for(j=0; j<i; j++) {
			free(tab[j]);
			tab[j]=NULL;
		}
		free(tab);
	}
	
	 
	recorder_free(lig_rec);
  	recorder_free(col_rec);
	timer_free(t);

	return EXIT_SUCCESS;
}


