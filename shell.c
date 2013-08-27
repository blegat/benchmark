#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#include "benchmark.h"

#define MAX_SIZE 500
//#define BM_USE_TIMES

int main (int argc, char *argv[])  {
	timer *t = timer_alloc();
	recorder *bash_rec = recorder_alloc("shell-bash.csv");
 	recorder *prog_rec = recorder_alloc("shell-prog.csv");

	char * progr = "./shell-program ";
	char * bash =  "./shell-bash.sh ";
	char  nbr[7];
 
	int i,a;

	char*argProgr = (char *) malloc(24*sizeof(char));
	char*argBash = (char *) malloc(24*sizeof(char));
	

	for(i=1; i<MAX_SIZE; i+=10) {

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
