#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#include "benchmark.h"

#define MAX_SIZE 100
//#define BM_USE_TIMES

/**
	\brief Ce programme compare le temps d'exécution d'un script et d'un programme C qui effectue un nombre "N" d'instruction.
	
	Ce programme utilise deux programmes annexes : 
		* shell-program : C'est un programme C qui effectue le nombre d'instruction donné en paramètre
		* shell-bash.sh : C'est un script qui effectue le nombre d'instruction donné en paramètre
*/
int main (int argc, char *argv[])  {
	// Déclare un timer, ainsi que deux recorder qui vont contenir les résultats de l'exécution du programme
	timer *t = timer_alloc();
	recorder *bash_rec = recorder_alloc("shell-bash.csv");
 	recorder *prog_rec = recorder_alloc("shell-prog.csv");

	// Nom du programme C et du script à comparer
	char * progr = "./shell-program ";
	char * bash =  "./shell-bash.sh ";
	
	// Variable contenant la transfor
	char  nbr[7];
 
	int i;

	char*argProgr = (char *) malloc(24*sizeof(char));
	char*argBash = (char *) malloc(24*sizeof(char));
	

	for(i=1; i<MAX_SIZE; i+=1) {

		snprintf(nbr, 7, "%d", i);

		strncpy(argProgr, progr, 24);
		strncpy(argBash, bash, 24);

		strncat(argProgr, nbr, 7);
		strncat(argBash, nbr, 7);


		start_timer(t);
		a = system(argProgr);
		write_record(prog_rec, i, stop_timer(t));


		start_timer(t);
		a = system(argBash);
		write_record(bash_rec, i, stop_timer(t));

	}

	recorder_free(bash_rec);
  	recorder_free(prog_rec);
	timer_free(t);

  return EXIT_SUCCESS;
}
