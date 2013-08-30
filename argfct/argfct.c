/**
	\file argfct.c
	\brief Ce programme compare le temps nécessaire pour passer un argument de N byte à une fonction et celui nécessaire pour passer le pointeur vers cet argument

	Compare donc les performances en passant des arguments de plus en plus gros sur le stack contre un pointeur de taille fixe
	
	Note : Pour ne pas polluer les tests, il a été décidé de ne pas utiliser de pointeurs de fonction.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#include "benchmark.h"

#define MAX_SIZE 3
#define TURN 100000000

//#define BM_USE_TIMES

/** Structure dont la taille fait 1byte */
struct arg1   { char s[1];   };
/** Structure dont la taille fait 2byte */
struct arg2   { char s[2];   };
/** Structure dont la taille fait 4byte */
struct arg4   { char s[4];   };
/** Structure dont la taille fait 8byte */
struct arg8   { char s[8];   };
/** Structure dont la taille fait 16byte */
struct arg16  { char s[16];  };
/** Structure dont la taille fait 32byte */
struct arg32  { char s[32];  };
/** Structure dont la taille fait 64byte */
struct arg64  { char s[64];  };
/** Structure dont la taille fait 128byte */
struct arg128 { char s[128]; };

void fctpt(void* arg) {}

void fctval1  (struct arg1 arg)   {}
void fctval2  (struct arg2 arg)   {}
void fctval4  (struct arg4 arg)   {}
void fctval8  (struct arg8 arg)   {}
void fctval16 (struct arg16 arg)  {}
void fctval32 (struct arg32 arg)  {}
void fctval64 (struct arg64 arg)  {}
void fctval128(struct arg128 arg) {}


int main (int argc, char *argv[])  {
	timer *t = timer_alloc();
	recorder *val_rec = recorder_alloc("argfct-val.csv");
 	recorder *pt_rec = recorder_alloc("argfct-pt.csv");

	struct arg1 a1;
	struct arg2 a2;
	struct arg4 a4;
	struct arg8 a8;
	struct arg16 a16;
	struct arg32 a32;
	struct arg64 a64;
	struct arg128 a128;

	int corres[] = { sizeof(struct arg1), 
				sizeof(struct arg1), 
				sizeof(struct arg2), 
				sizeof(struct arg4), 
				sizeof(struct arg8), 
				sizeof(struct arg16), 
				sizeof(struct arg32), 
				sizeof(struct arg128)};

	long long int * timeval = calloc(8, sizeof(double));
	long long int * timept = calloc(8, sizeof(double));
	int i,j;

	//for(i=0; i<MAX_SIZE; i++) {

		//Preparation
		for(j=0; j<TURN; j++)
			fctpt((void*)&a1);
		for(j=0; j<TURN; j++)
			fctval1(a1);
		for(j=0; j<TURN; j++)
			fctpt((void*)&a4);


		// struct arg1
		start_timer(t);
		for(j=0; j<TURN; j++)
			fctpt((void*)&a1);
		timept[0] += (stop_timer(t)*1000/TURN);
		start_timer(t);
		for(j=0; j<TURN; j++)
			fctval1(a1);
		timeval[0] += (stop_timer(t)*1000/TURN);

		// struct arg2
		start_timer(t);
		for(j=0; j<TURN; j++)
			fctpt((void*)&a2);
		timept[1] += (stop_timer(t)*1000/TURN);
		start_timer(t);
		for(j=0; j<TURN; j++)
			fctval2(a2);
		timeval[1] += (stop_timer(t)*1000/TURN);

		// struct arg4
		start_timer(t);
		for(j=0; j<TURN; j++)
			fctpt((void*)&a4);
		timept[2] += (stop_timer(t)*1000/TURN);
		start_timer(t);
		for(j=0; j<TURN; j++)
			fctval4(a4);
		timeval[2] += (stop_timer(t)*1000/TURN);
	
		// struct arg8
		start_timer(t);
		for(j=0; j<TURN; j++)
			fctpt((void*)&a8);
		timept[3] += (stop_timer(t)*1000/TURN);
		start_timer(t);
		for(j=0; j<TURN; j++)
			fctval8(a8);
		timeval[3] += (stop_timer(t)*1000/TURN);

		// struct arg16
		start_timer(t);
		for(j=0; j<TURN; j++)
			fctpt((void*)&a16);
		timept[4] += (stop_timer(t)*1000/TURN);
		start_timer(t);
		for(j=0; j<TURN; j++)
			fctval16(a16);
		timeval[4] += (stop_timer(t)*1000/TURN);

		// struct arg32
		start_timer(t);
		for(j=0; j<TURN; j++)
			fctpt((void*)&a32);
		timept[5] += (stop_timer(t)*1000/TURN);
		start_timer(t);
		for(j=0; j<TURN; j++)
			fctval32(a32);
		timeval[5] += (stop_timer(t)*1000/TURN);
	
		// struct arg64
		start_timer(t);
		for(j=0; j<TURN; j++)
			fctpt((void*)&a64);
		timept[6] += (stop_timer(t)*1000/TURN);
		start_timer(t);
		for(j=0; j<TURN; j++)
			fctval64(a64);
		timeval[6] += (stop_timer(t)*1000/TURN);

		// struct arg128
		start_timer(t);
		for(j=0; j<TURN; j++)
			fctpt((void*)&a128);
		timept[7] += (stop_timer(t)*1000/TURN);
		start_timer(t);
		for(j=0; j<TURN; j++)
			fctval128(a128);
		timeval[7] += (stop_timer(t)*1000/TURN);
	//}

	for(i=0; i<8; i++) {
		write_record(pt_rec, corres[i], (long int) (timept[i]));//(MAX_SIZE)));
		write_record(val_rec, corres[i], (long int) (timeval[i]));//(MAX_SIZE)));
	}

	recorder_free(val_rec);
  	recorder_free(pt_rec);
	timer_free(t);

  return EXIT_SUCCESS;
}


