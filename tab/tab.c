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
	\brief Compare les performances lorsque l'on parcourt un tableau par ligne ou par colonne

	Ce programme additionne les valeurs d'un tableau en le parcourant :
		* Soit par colonne
		* Soit par ligne

	Note : Le plus rapide profite de la localité spatiale
*/
int main (int argc, char *argv[])  {
	// Déclare un timer, ainsi que deux recorder qui vont contenir les résultats de l'exécution du programme
	timer * t = timer_alloc();
	recorder * lig_rec = recorder_alloc("tab-lig.csv");
 	recorder * col_rec = recorder_alloc("tab-col.csv");

	int i,j;
	for(i=MULTIPLICATEUR; i<MAX_SIZE; i+=MULTIPLICATEUR) {
		
		// On crée le tableau
		int ** tab = malloc(i*sizeof(int*));
		for(j=0; j<i; j++) 
			tab[j] = malloc(i*sizeof(int));
		

		int m,n, res=0;

		// On parcours par colonne
		start_timer(t);
		for(m=0; m<i; m++) {
			for(n=0; n<i; n++) {		
				res+= tab[n][m];
			}
		}
		write_record_n(lig_rec, i/MULTIPLICATEUR, stop_timer(t), MULTIPLICATEUR);

		// On parcours par ligne
		start_timer(t);
		for(m=0; m<i; m++) {
			for(n=0; n<i; n++) {
				res+= tab[m][n];
			}
		}
		write_record_n(col_rec, i/MULTIPLICATEUR, stop_timer(t), MULTIPLICATEUR);

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
