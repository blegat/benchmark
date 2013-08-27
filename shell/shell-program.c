#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>



int main (int argc, char *argv[])  {
	
	int i, size = atoi(argv[1]);
	int a=0;


	for(i=0; i<size; i++) {
		a= a+1;
	}

  return EXIT_SUCCESS;
}
