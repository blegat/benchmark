/**
	\file calloc.c
	\brief Ce programme compare l'utilisation de calloc ou de malloc pour allouer de l'espace mémoire

	Ce programme va donc allouer de l'espace avec calloc d'une part et malloc d'autre part, en comparant les temps nécessaires à chacun.	

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "benchmark.h"

//Time by default (4)


#define MULTIPLICATEUR 10000
#define MAX 800000


int main (int argc, char *argv[])  {
	// Déclare un timer, ainsi que deux recorder qui vont contenir les résultats de l'exécution du programme
	timer * t = timer_alloc();
	recorder * mal_rec = recorder_alloc("malloc.csv");
 	recorder * cal_rec = recorder_alloc("calloc.csv");

	void* res;
	int i;
	for(i=MULTIPLICATEUR; i< MAX; i+=MULTIPLICATEUR) {
		start_timer(t);
		res = malloc(i);
		write_record(mal_rec, i+MULTIPLICATEUR, stop_timer(t));
		free(res);

		start_timer(t);
		res = calloc(i,1);
		write_record(cal_rec, i+MULTIPLICATEUR, stop_timer(t));
		free(res);
	}

	recorder_free(mal_rec);
  	recorder_free(cal_rec);
	timer_free(t);

  return EXIT_SUCCESS;
}
