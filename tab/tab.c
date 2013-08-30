/**
	\file tab.c
	\brief Compare les performances lorsque l'on parcourt un tableau par ligne ou par colonne

	Ce programme additionne les valeurs d'un tableau en le parcourant :
		* Soit par colonne
		* Soit par ligne

	Lorsque l'on spécifie un argument pour effectuer les perfs, sur les lignes ou les colonnes, on n'écrit pas dans les records et on ne n'exécute soit les lignes soit les colonnes selon l'arguments reçu.

	Note : Le plus rapide profite de la localité spatiale
*/

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

timer * t ;
recorder * lig_rec;
recorder * col_rec;
	

/**
	On parcourt le double tableau par ligne, en retournant le temps nécessaire au parcours du tableau
*/
long int ligne(int i, timer *t, int ** tab) {
	int m,n,res=0;
	start_timer(t);
	for(m=0; m<i; m++) 
		for(n=0; n<i; n++) 	
			res+= tab[n][m];
		
	return stop_timer(t);
}

/**
	On parcourt le double tableau par colonne, en retournant le temps nécessaire au parcours du tableau
*/
long int colonne(int i, timer *t, int ** tab) {
	int m,n,res=0;
	start_timer(t);
	for(m=0; m<i; m++) 
		for(n=0; n<i; n++) 	
			res+= tab[m][n];
		
	 return stop_timer(t);
}


int main (int argc, char *argv[])  {
	// Verification des arguments
  	int perfligne = argc>1 && strncmp(argv[1], "--ligne", 8);
	int perfcolonne = argc>1 &&strncmp(argv[1], "--colonne", 10);

	timer * t = timer_alloc();
	if(!perfligne && !perfcolonne) {
		// Déclare un timer, ainsi que deux recorder qui vont contenir les résultats de l'exécution du programme
		lig_rec = recorder_alloc("tab-lig.csv");
	 	col_rec = recorder_alloc("tab-col.csv");
	}

	int i,j;
	long int rescol, reslig;
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
			reslig = ligne(i, t, tab);
		if(!perfligne)
			rescol = colonne(i, t, tab);

		if(!perfligne && !perfcolonne) {
			write_record_n(col_rec, i/MULTIPLICATEUR, rescol, MULTIPLICATEUR);
			write_record_n(lig_rec, i/MULTIPLICATEUR, reslig, MULTIPLICATEUR);
		}

		//On libère la mémoire
		for(j=0; j<i; j++) {
			free(tab[j]);
			tab[j]=NULL;
		}
		free(tab);
	}
	
	if(!perfligne && !perfcolonne) {
		recorder_free(lig_rec);
	  	recorder_free(col_rec);
		timer_free(t);
	}

	return EXIT_SUCCESS;
}
