/**
	\file shell.c
	\brief Ce programme compare le temps d'exécution d'un script et d'un programme C qui effectue un nombre "N" d'instruction.
	
	Ce programme utilise deux programmes annexes : 
		* shell-program : C'est un programme C qui effectue le nombre d'instruction donné en paramètre.
		* shell-bash.sh : C'est un script qui effectue le nombre d'instruction donné en paramètre.

	Pour exécuter chaque programme, l'appel systeme "system" est utilisé. Celui-ci exécute la commande passé en paramètre.
	Typiquement, il recevra 
		* "./shell-program 500" : Pour exécuter 500 instructions avec le programme C
		* "./shell-bash.sh 250" : Pour exécuter 250 instructions avec le script

	Le paramètre passé à "system" contient donc deux parties. D'une part ce qu'il faut exécuter, d'autre part le nombre d'instruction.

	Les résultats sont écrit dans :
		* shell-bash.csv : Pour les temps d'exécution du script
		* shell-prog.csv : Pour les temps d'exécution du programme
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#include "benchmark.h"

#define MAX_SIZE 100

//#define BM_USE_TIMES


int main (int argc, char *argv[])  {
	// Déclare un timer, ainsi que deux recorder qui vont contenir les résultats de l'exécution du programme
	timer *t = timer_alloc();
	recorder *bash_rec = recorder_alloc("shell-bash.csv");
 	recorder *prog_rec = recorder_alloc("shell-prog.csv");

	// Nom du programme C et du script à comparer
	char * progr = "./shell-program ";
	char * bash =  "./shell-bash.sh ";
	
	// Cette variable contient la transformation du nombre d'instruction (i).
	// Puisque cette variable est un int, 7 caractères suffisent à sa représentation.
	char  nbr[7];
	int i;

	// Allocation des emplacements contenant la commande à exécuter
	char*argProgr = (char *) malloc(24*sizeof(char));
	char*argBash = (char *) malloc(24*sizeof(char));

	if(argProgr == NULL || argBash == NULL) 
		exit(EXIT_FAILURE);

	
	for(i=1; i<MAX_SIZE; i+=1) {
		// Convertit "i" en char* et le place dans nbr
		snprintf(nbr, 7, "%d", i);

		// Concatene les deux parties de la commande 
		strncpy(argProgr, progr, 24);
		strncpy(argBash, bash, 24);
		strncat(argProgr, nbr, 7);
		strncat(argBash, nbr, 7);

		// Commence le timer et lance la commande, puis écrit le résultat dans le record approprié
		start_timer(t);
		system(argProgr);
		write_record(prog_rec, i, stop_timer(t));

		start_timer(t);
		system(argBash);
		write_record(bash_rec, i, stop_timer(t));
	}

	// Libère la mémoire
	recorder_free(bash_rec);
  	recorder_free(prog_rec);
	timer_free(t);

  return EXIT_SUCCESS;
}
