/*****************************************
 * pipe.c
 *
 * Temps de transfert de données par pipe
 *****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <err.h>
#include <unistd.h>
#include <sys/wait.h>

#include "benchmark.h"

#define SIZE_MAX 65536
// pipe buffer is 64k => more is impossible

/**
 * \brief Alloue un string et l'init à 'a'
 *
 * \param size Taille du string à allouer
 *
 * \return Pointeur vers string
 */

char* ipsum (int size) {
	char* ipsum = (char*)malloc(sizeof(char)*size);
	if (ipsum == NULL) err(1, "erreur de malloc dans ipsum");
	memset(ipsum,'a',size);
	
	ipsum[size-1] = '\0';
	
	return ipsum;
	
}

/**
 * \brief Envoie un string dans un pipe
 *
 * \param size Taille du string à allouer
 *
 * \param fd FileDescriptor du pipe
 *
 * Fonction que l'on donne au processus fils. Write un string de taille `size` 
 * dans un pipe vers le père
 */

int send (int size, int* fd) {
	
	if (size == 1) close(fd[0]);
	
	char* message = ipsum(size);
	if (message == NULL) err(1, "erreur de malloc dans send");
	
	int sent = write(fd[1], message, size);
	if (sent != size ) err(1,"erreur de write dans send");
	
	free(message);
	
	//if (size == SIZE_MAX) close(fd[1]);
	
	return 0;
}

/**
 * \brief Reçoit et renvoie un string dans un pipe
 *
 * \param size Taille du string à allouer
 *
 * \param fdin FileDescriptor du pipe in
 *
 * \param fdout FileDescriptor du pipe out
 *
 * Fonction que l'on donne au processus père. Read un string de taille `size`
 * dans un pipe input, puis renvoie ce string vers le fils avec le pipe output
 */

int respond (int size, int* fdin, int* fdout) {
	
	if (size == 1) close(fdout[0]);
	if (size == 1) close(fdin[1]);
	
	char* buffer = (char*)malloc(sizeof(char)*size);
	if (buffer == NULL) err(1, "erreur de malloc dans respond");
	
	int received = read(fdin[0], buffer, size);
	if (received != size ) err(1,"erreur de read dans respond");
	
	//puts(buffer);
	
	int sent = write(fdout[1], buffer, size);
	if (sent != size ) err(1,"erreur de write dans respond");
	
	free(buffer);
	
	//if (size == SIZE_MAX) close(fdout[1]);
	//if (size == SIZE_MAX) close(fdin[0]);
	
	return 0;
}

/**
 * \brief Recois un string dans un pipe
 *
 * \param size Taille du string à allouer
 *
 * \param fd FileDescriptor du pipe
 *
 * Fonction que l'on donne au processus fils. Read un string de taille `size`
 * dans un pipe vers le père
 */

int receive (int size, int* fd) {
	
	if (size == 1) close(fd[1]);
	
	char* buffer = (char*)malloc(sizeof(char)*size);
	if (buffer == NULL) err(1, "erreur de malloc dans receive");
	
	int received = read(fd[0], buffer, size);
	if (received != size ) err(1,"erreur de read dans receive");
	
	//puts(buffer);
	
	free(buffer);
	
	//if (size == SIZE_MAX) close(fd[0]);
	
	return 0;
}

/**
 * \brief Enregistre les vitesses de transfert dans des pipes entre processus
 *
 * Le but de ce banchmark est de voir l'évolution du temps mis à transferer des
 * données par pipe entre 2 processus. On effectue le transfert de fichiers de 
 * plus en plus gros et en enregistre le temps mis dans un fichier .csv.
 */

int main (int argc, char* argv[]) {
	int fdin[2];
	int fdout[2];
	pipe(fdin);
	pipe(fdout);
	
	timer *t = timer_alloc();
	recorder *pipe_rec = recorder_alloc("pipe.csv");
	//start_timer(t);
	pid_t pid = fork();
	
	if (pid == 0) {
		int i;
		for (i=1; i<=SIZE_MAX; i*=2) {
			start_timer(t);
			int j;
			for (j=0;j<10000;j++) {
				send(i,fdin);
				receive(i,fdout);
			}
			write_record_n(pipe_rec,i,stop_timer(t),SIZE_MAX);
		}
		exit(0);
	} else if (pid < 0 ) {
		err(pid, "erreur au fork");
	} else {
		int i;
		for (i=1; i<=SIZE_MAX; i*=2) {
			//start_timer(t);
			int j;
			for (j=0;j<10000;j++) {
				respond(i,fdin,fdout);
			}
			//write_record_n(pipe_rec,i,stop_timer(t),SIZE_MAX);
		}
	}
	
	recorder_free(pipe_rec);
	timer_free(t);
	
	return 0;
}
