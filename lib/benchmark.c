/**
 * \file benchmark.c
 * \brief fonctions utiles pour écrire les `.csv` et mesurer le temps
 *
 * Cette libraire contient `timer` pour mesurer le temps et `recorder`
 * pour écrire les temps dans un fichier au format `.csv` qu'on peut
 * plotter facilement avec `gnuplot`.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>
#include "benchmark.h"

/*  _____ _
 * |_   _(_)_ __ ___   ___ _ __
 *   | | | | '_ ` _ \ / _ \ '__|
 *   | | | | | | | | |  __/ |
 *   |_| |_|_| |_| |_|\___|_|
 */

/**
 * \brief `timer` permet de mesurer le temps écoulé entre deux moments
 *
 * Il implémente plusieurs manières de mesurer le temps.
 * Par défaut, le temps est calculé à l'aide de `gettimeofday`,
 * c'est donc le temps *réel* mais en ajoutant par exemple
 *
 *     CFLAGS = -DBM_USE_CLOCK
 * dans le `Makefile`, `timer` utilisera `clock` à la place de `gettimeofday`
 * Les méthodes disponibles sont
 * * `BM_USE_CLOCK_GETTIME_RT` calcule le temps *réel* avec `clock_gettime`
 * * `BM_USE_CLOCK_GETTIME` calcule le temps *user* + le temps *système* avec
 *   `clock_gettime`
 * * `BM_USE_CLOCK` calcule le temps *user* + le temps *système* avec
 *   `clock`
 * * `BM_USE_TIMES` calcule le temps *user* + le temps *système* avec
 *   `times`
 */
struct timer {
#if   defined(BM_USE_CLOCK_GETTIME) || defined(BM_USE_CLOCK_GETTIME_RT)
  struct timespec start;
#elif defined(BM_USE_CLOCK)
  clock_t start;
#elif defined(BM_USE_TIMES)
  struct tms start;
  long clock_ticks;
#else
  struct timeval start;
#endif
};

/**
 * \brief Alloue un `timer`
 *
 * En cas de succès, retourne un `timer`,
 * en cas d'erreur, affiche un message sur `stderr`
 * et `exit`
 */
timer *timer_alloc () {
  timer *t = (timer *) malloc(sizeof(timer));
  if (t == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
#if defined(BM_USE_TIMES)
  t->clock_ticks = sysconf(_SC_CLK_TCK);
  if (t->clock_ticks == -1) {
    free(t);
    perror("sysconf");
    exit(EXIT_FAILURE);
  }
#endif
  return t;
}

/**
 * \brief Stoque le temps actuel comme début de la mesure dans `t`
 *
 * \param t Le temps dans lequel on stoque le temps de début
 *
 * En cas d'erreur, affiche un message sur `stderr` et `exit`
 */
void start_timer (timer *t) {
#if   defined(BM_USE_CLOCK_GETTIME)
  if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t->start)) {
    perror("clock_gettime");
    exit(EXIT_FAILURE);
  }
#elif defined(BM_USE_CLOCK_GETTIME_RT)
  if (clock_gettime(CLOCK_REALTIME, &t->start)) {
    perror("clock_gettime");
    exit(EXIT_FAILURE);
  }
#elif defined(BM_USE_CLOCK)
  t->start = clock();
  if (t->start == (clock_t) -1) {
    perror("clock");
    exit(EXIT_FAILURE);
  }
#elif defined(BM_USE_TIMES)
  if (times(&t->start) == -1) {
    perror("times");
    exit(EXIT_FAILURE);
  }
#else
  gettimeofday(&t->start, NULL);
#endif
}

#define BILLION 1000000000
//               123456789

#define GETTIME_TO_NSEC(time_gettime) \
  (((long) time_gettime.tv_sec) * BILLION + time_gettime.tv_nsec)
#define CLOCK_TO_NSEC(time_clock) \
  ((((long) time_clock) * BILLION) / CLOCKS_PER_SEC)
#define TIMES_TO_NSEC(time_times) \
  ((((long) time_times.tms_utime + time_times.tms_stime) * BILLION) / t->clock_ticks)
#define GTOD_TO_NSEC(time_gtod) \
  (((long) time_gtod.tv_sec) * BILLION + time_gtod.tv_usec * 1000)

/**
 * \brief Retourne le temps en nanosecondes depuis le début de mesure dans `t`
 *
 * \param t Le temps dans lequel on a stoqué le temps de début
 *
 * En cas d'erreur, affiche un message sur `stderr` et `exit`
 */
long int stop_timer (timer *t) {
  // time_t is only a 32 bits int on 32 bits machines
#if   defined(BM_USE_CLOCK_GETTIME) || defined(BM_USE_CLOCK_GETTIME_RT)
  struct timespec end;
  //if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end)) {
  if (clock_gettime(CLOCK_REALTIME, &end)) {
    perror("clock_gettime");
    exit(EXIT_FAILURE);
  }
  long total = GETTIME_TO_NSEC(end) - GETTIME_TO_NSEC(t->start);
#elif defined(BM_USE_CLOCK)
  clock_t end = clock();
  if (end == (clock_t) -1) {
    perror("clock");
    exit(EXIT_FAILURE);
  }
  long total = CLOCK_TO_NSEC(end) - CLOCK_TO_NSEC(t->start);
#elif defined(BM_USE_TIMES)
  struct tms end;
  if (times(&end) == -1) {
    perror("times");
    exit(EXIT_FAILURE);
  }
  long total = TIMES_TO_NSEC(end) - TIMES_TO_NSEC(t->start);;
#else
  struct timeval end;
  gettimeofday(&end, NULL);
  long total = GTOD_TO_NSEC(end) - GTOD_TO_NSEC(t->start);
#endif
  return total;
}

/**
 * \brief Retourne le temps en nanosecondes depuis le début de mesure dans `t`
 *
 * \param t Le temps dans lequel on a stoqué le temps de début
 *
 * En cas d'erreur, affiche un message sur `stderr` et `exit`
 */
void timer_free (timer *t) {
  free(t);
}

/*  ____                        _
 * |  _ \ ___  ___ ___  _ __ __| | ___ _ __
 * | |_) / _ \/ __/ _ \| '__/ _` |/ _ \ '__|
 * |  _ <  __/ (_| (_) | | | (_| |  __/ |
 * |_| \_\___|\___\___/|_|  \__,_|\___|_|
 */

/**
 * \brief Temps nécessaire pour effectuer `start_timer` et `stop_timer`
 *        sans rien faire entre
 */
volatile long int overhead = -1;
/**
 * \brief Mets à jours l'`overhead`
 */
void update_overhead() {
  timer *t = timer_alloc();
  start_timer(t);
  overhead = stop_timer(t);
  timer_free(t);
  printf("overhead updated: %ld\n", overhead);
}
long int get_overhead () {
  if (-1 == overhead) {
    update_overhead();
  }
  return overhead;
}

/**
 * \brief `recorder` écrit les temps dans un fichier `.csv`
 */
struct recorder {
  FILE *output;
  long int overhead;
};

/**
 * \brief Alloue un `recorder`
 *
 * Les données seront écrites dans le fichier `filename`.
 * En cas d'erreur, il `exit` avec `EXIT_FAILURE`.
 */
recorder *recorder_alloc (char *filename) {
  recorder *rec = (recorder *) malloc(sizeof(recorder));
  if (rec == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  rec->output = fopen(filename, "w");
  if (rec->output == NULL) {
    perror("fopen");
    free(rec);
    exit(EXIT_FAILURE);
  }
  rec->overhead = get_overhead();
  return rec;
}

/**
 * \brief Écris le temps `time` en correspondance avec `x`
 *
 * L'`overhead` est d'abord retiré de `time`
 * En cas d'erreur, il `exit` avec `EXIT_FAILURE`
 *
 * \param rec le `recorder` dans lequel écrire, il est supposé non-`NULL`
 * \param x l'abscisse
 * \param time le temps à écrire en ordonnée
 */
void write_record (recorder *rec, long int x, long int time) {
  write_record_n(rec, x, time, 1);
}

/**
 * \brief Comme `write_record` mais divise `time` par `n` après
 *        avoir retiré l'`overhead`
 */
void write_record_n (recorder *rec, long int x, long int time, long n) {
  fprintf(rec->output, "%ld, %ld\n", x, (time - rec->overhead) / n);
}


/**
 * \brief Libère toutes les resources utilisées par `rec`
 *
 * \param rec le recorder auquel il faut libérer les resources
 */
void recorder_free (recorder *rec) {
  fclose(rec->output);
  free(rec);
}
