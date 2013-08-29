/*************************************************************
 * amdahl.c
 *
 * Benchmark du gain de vitesse obtenu par la parallelisation
 *************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <err.h>
#include <unistd.h>
#include <sys/wait.h>

#include "benchmark.h"

#define NTHREAD 32 //!< Nombre max de threads/processus à utiliser
#define NLENGTH 64000 //!< Taille du tableau à tester

/*
 *   _____ _                   _
 *  / ____| |                 | |
 * | (___ | |_ _ __ _   _  ___| |_ _   _ _ __ ___
 *  \___ \| __| '__| | | |/ __| __| | | | '__/ _ \
 * _ ___) | |_| |  | |_| | (__| |_| |_| | | |  __/
 * |_____/ \__|_|   \__,_|\___|\__|\__,_|_|  \___|
 */

/**
 * \brief Sert à stocker les arguments passés aux threads
 *
 * `scanargs` est la structure utilisé pour faire passer les arguments aux threads.
 * `start` est l'index du premier élement du tableau `array` que dois scaner le
 * thread, et `stop` en est le dernier.
 */

typedef struct scanargs {
	int start;
	int stop;
	int* array;
} scanargs;

/**
 * \brief Sert à stocker les valeurs de retour des threads
 *
 * `result` est la structure utilisé pour stocker les valeurs renvoyés par les
 * avec `pthread_exit()` . `count` est le nombre max de facteurs premiers qu'un
 * thread a trouvé pour une valeur du tableau et `index` est l'index de cette
 * valeur dans `array` .
 */

typedef struct result {
	int count;
	int index;
} result;

/* 
 *  _____      _                  ______         _
 * |  __ \    (_)                |  ____|       | |
 * | |__) | __ _ _ __ ___   ___  | |__ __ _  ___| |_ ___  _ __ ___
 * |  ___/ '__| | '_ ` _ \ / _ \ |  __/ _` |/ __| __/ _ \| '__/ __|
 * | |   | |  | | | | | | |  __/ | | | (_| | (__| || (_) | |  \__ \
 * |_|   |_|  |_|_| |_| |_|\___| |_|  \__,_|\___|\__\___/|_|  |___/
 */

/**
 * \brief Calcul les facteurs premiers de `n`
 *
 * \param n Le nombre duquel on veut trouver les facteurs premiers
 *
 * \return Le nombre de facteurs premiers trouvé
 *
 * Calcul les facteurs premiers de `n` par la methode de brute force
 */

int primeFactors(int n) {
	int i,count=0;
	for(i=2;i<=n;i++)
	{
		if(n%i==0)
		{
			count++;
			n=n/i;
			i--;
			if(n==1)
				break;
		} 
	} 
	return count;
}

/*
 *   _____
 *  / ____|
 * | (___   ___ __ _ _ __
 *  \___ \ / __/ _` | '_ \
 *  ____) | (_| (_| | | | |
 * |_____/ \___\__,_|_| |_|
 */

/**
 * \brief Calcul les facteurs premiers dans une portion de tableau de Integer
 *
 * \param param Structure `scanargs` contenant le tableau a scanner et les index de
 * la portion à scanner
 *
 * \return Une structure `result` contenant l'index du nombre avec le plus de
 * facteurs premiers et le nombre de ces facteurs
 *
 * Fonction à faire passer à un thread. Scan une portion d'un tableau à la
 * du nombre avec le plus de facteurs premiers, et renvois cette valeur ainsi
 * que l'index où se trouve ce nombre.
 */

void* scan(void* param) {
	scanargs* args = (scanargs*)param;
	result *res = (result*)malloc(sizeof(result));
	res->count = 0;
	res->index = 0;
	
	int i,p;
	for (i = args->start; i<=args->stop; i++) {
		p = primeFactors(args->array[i]);
		//printf("number: %d ; primes: %d ; index: %d\n", args->array[i],p,i);
		if (p>res->count) {
			res->count = p;
			res->index = i;
		}
	}
	pthread_exit((void*)res);
}

/*
 *  __  __       _
 * |  \/  |     (_)
 * | \  / | __ _ _ _ __
 * | |\/| |/ _` | | '_ \
 * | |  | | (_| | | | | |
 * |_|  |_|\__,_|_|_| |_|
 */

/**
 * \brief Enregistre les temps de calcul nécessaire à le nombre avec le plus de
 * facteurs premiers
 *
 * On va tester la loi d'Amdahl et donc vérifier l'avantage au niveau de la vitesse
 * gagner grâce à la parallelisation d'un code CPU intensif tel que la recherche
 * du nombre ayant la plus de facteurs premiers dans un tableau de valeurs 
 * aléatoires.
 * La recherche est faite avec 1 puis 2 puis 3 etc... threads/processus, avec à
 * chaque fois le temps nécessaire enregistrer dans un fichier .csv puis afficher à 
 * la fin sous forme de graphe.
 */


int main (int argc, char* argv[]) {
	
	timer *t = timer_alloc();
	recorder *thread_rec = recorder_alloc("thread.csv");
	recorder *proc_rec = recorder_alloc("proc.csv");
	// on init tous les `recorders` et le timer
	
	srand (1337);
	int* array = (int*)malloc(sizeof(int)*NLENGTH);
	if (array == NULL) err(1,"erreur malloc");
	int i;
	for (i=0; i<NLENGTH; i++) {
		array[i] = rand() % NLENGTH;
	}
	// on génère le tableau de int aléatoires
	
	
	/*
	 *  _______ _                        _
	 * |__   __| |                      | |
	 *    | |  | |__  _ __ ___  __ _  __| |___
	 *    | |  | '_ \| '__/ _ \/ _` |/ _` / __|
	 *    | |  | | | | | |  __/ (_| | (_| \__ \
	 *    |_|  |_| |_|_|  \___|\__,_|\__,_|___/
     */
	
	for (i = 1; i<=NTHREAD; i=i+1) {
		start_timer(t);
		// on lance le chronomètre
		int error = 0;
		
		pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t)*i);
		if (threads == NULL) err(1,"erreur malloc");
		scanargs** args = (scanargs**)malloc(sizeof(scanargs*)*i);
		if (args == NULL) err(1,"erreur malloc");
		result* res[i];
		// init des tableaux d'arguments, de threads, et de resultats
		
		int j;
		for (j=0; j<i; j++) {
			args[j] = (scanargs*)malloc(sizeof(scanargs));
			if (args[j] == NULL) err(1,"erreur malloc");
			args[j]->start = j*(NLENGTH/i);
			args[j]->stop = (j+1)*(NLENGTH/i)-1;
			args[j]->array = array;
			// remplissage de la structure argument avec le segment à scanner par le thread
			error = pthread_create(&threads[j],NULL,&scan,(void*)args[j]);
			if(error!=0) err(error,"erreur create");
			// lancement du thread
		}
		
		for (j=0; j<i; j++) {
			error=pthread_join(threads[j],(void**)&res[j]);
			if(error!=0) err(error,"erreur create");
			// join du thread et réception de la structure resultat
		}
		
		/*
		 * Analyse des resultats de tous les threads
		 */
		int index = -1;
		int max = 0;
		for (j=0; j<i; j++) {
			if (max<res[j]->count) {
				max = res[j]->count;
				index = res[j]->index;
				free(res[j]);
				free(args[j]);
			}
		}
		
		printf("%d\n",index);
		write_record_n(thread_rec,i,stop_timer(t),NTHREAD);
		//sauvegarde du temps
		
		free(threads);
		free(args);
	}
	
	puts("proc");
	
	/*
	 *  _____
	 * |  __ \
	 * | |__) | __ ___   ___ ___  ___ ___  ___  ___
	 * |  ___/ '__/ _ \ / __/ _ \/ __/ __|/ _ \/ __|
	 * | |   | | | (_) | (_|  __/\__ \__ \  __/\__ \
	 * |_|   |_|  \___/ \___\___||___/___/\___||___/
     *
	 *
	 * Meme chose que les threads mais avec des processus cette fois ci
	 */
	
	for (i = 1; i<=NTHREAD; i=i+1) {
		
		start_timer(t);
		int error = 0;
		pid_t pid[i];
		int fd[i][2];
		
		int j;
		for (j=0; j<i; j++) {
			int start = j*(NLENGTH/i);
			int stop = (j+1)*(NLENGTH/i)-1;
			// on définit le segment à scanner
			pipe(fd[j]);
			// on init le pipe entre le père et fils. Il sert à ce que le fils renvoie sa réponse au père
			pid[j] = fork();
			
			if (pid[j] == 0) {
				// on est dans le fils
				int k,p,index,count=0;
				for (k = start; k<=stop; k++) {
					p = primeFactors(array[k]);
					//printf("number: %d ; primes: %d ; index: %d\n", args->array[i],p,i);
					if (p>count) {
						count = p;
						index = k;
					}
				}
				
				//printf("index: %d count: %d\n",index,count);
				
				index = index*1000;
				index += count;
				// vu qu'on a 2 int à renvoyer, on les combine en 1 int à déchiffre par le père
				//printf("index: %d\n",index);
				
				close(fd[j][0]);
				write(fd[j][1], &index, sizeof(index));
				// envoie de la réponse par pipe
				close(fd[j][1]);
				
				exit(0);
			} else if (pid[j] < 0) {
				err(-1,"erreur de fork");
			}
		}
		int max = 0,index,ret;
		for (j=0; j<i; j++) {
			
			close(fd[j][1]);
			read(fd[j][0], &ret, sizeof(ret));
			//réception de la réponse des fils
			close(fd[j][0]);
			
			pid[j] = waitpid(pid[j],NULL,0);
			if(pid[j]<1)err(-1,"erreur waitpid");
			//printf("ret: %d\n",ret);
			//analyse des résultats
			if (max < ret%1000) {
				max = ret%1000;
				index = ret/1000;
			}
		}
		
		printf("%d\n",index);
	    /*
		 *int it = i;
		 *int numCPU = sysconf( _SC_NPROCESSORS_ONLN );
		 *if (it > numCPU ) it = numCPU;
		 */
		write_record_n(proc_rec,i,stop_timer(t),NTHREAD);
		// ecriture du temps
	}
	
	recorder_free(thread_rec);
	recorder_free(proc_rec);
	timer_free(t);
	free(array);
	// free de nos structures
}