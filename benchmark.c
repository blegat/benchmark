/**************************************
 * benchmark.c
 *
 * Des fonctions simples de benchmark.
 **************************************/

//#define BM_USE_CLOCK_GETTIME
#define BM_USE_CLOCK
//#define BM_USE_TIMES

#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <time.h>
//#include <error.h>
#include <unistd.h>
#include "benchmark.h"

/*  _____ _
 * |_   _(_)_ __ ___   ___ _ __
 *   | | | | '_ ` _ \ / _ \ '__|
 *   | | | | | | | | |  __/ |
 *   |_| |_|_| |_| |_|\___|_|
 */

struct timer {
#if   defined(BM_USE_CLOCK_GETTIME)
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

void start_timer (timer *t) {
#if   defined(BM_USE_CLOCK_GETTIME)
  if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t->start)) {
  //if (clock_gettime(CLOCK_REALTIME, &t->start)) {
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

#define CLOCK_TO_NSEC(time_clock) \
  ((((long) time_clock) * 1000000000) / CLOCKS_PER_SEC)
#define TIMES_TO_NSEC(time_times) \
  ((((long) time_times.tms_utime + time_times.tms_stime) * 1000000000) / t->clock_ticks)

long int stop_timer (timer *t) {
  // time_t is only a 32 bits int on 32 bits machines
#if   defined(BM_USE_CLOCK_GETTIME)
  struct timespec end;
  if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end)) {
  //if (clock_gettime(CLOCK_REALTIME, &end)) {
    perror("clock_gettime");
    exit(EXIT_FAILURE);
  }
  long total = ((long) end.tv_sec) * 1000000000 + end.tv_nsec -
    ((long) t->start.tv_sec) * 1000000000 + t->start.tv_nsec;
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
  long total = ((long) end.tv_sec) * 1000000 + end.tv_usec -
    ((long) t->start.tv_sec) * 1000000 + t->start.tv_usec;
#endif
  return total;
}

void *timer_free (timer *t) {
  free(t);
}

/*  ____                        _
 * |  _ \ ___  ___ ___  _ __ __| | ___ _ __
 * | |_) / _ \/ __/ _ \| '__/ _` |/ _ \ '__|
 * |  _ <  __/ (_| (_) | | | (_| |  __/ |
 * |_| \_\___|\___\___/|_|  \__,_|\___|_|
 */

volatile long int overhead = -1;
long int update_overhead() {
  timer *t = timer_alloc();
  start_timer(t);
  overhead = stop_timer(t);
  timer_free(t);
  printf("overhead updated: %ld\n", overhead);
  printf("clocks per sec: %d\n", CLOCKS_PER_SEC);
}
long int get_overhead () {
  if (-1 == overhead) {
    update_overhead();
  }
  return overhead;
}

//typedef struct recorder recorder;
struct recorder {
  FILE *output;
  long int overhead;
};

recorder *recorder_alloc (char *filename) {
  recorder *rec = (recorder *) malloc(sizeof(recorder));
  if (rec == NULL) {
    perror("malloc");
    return NULL;
  }
  rec->output = fopen(filename, "w");
  if (rec->output == NULL) {
    perror("fopen");
    free(rec);
    return NULL;
  }
  rec->overhead = get_overhead();
  return rec;
}

void write_record (recorder *rec, long int x, long int time) {
  write_record_n(rec, x, time, 1);
}

void write_record_n (recorder *rec, long int x, long int time, long n) {
  fprintf(rec->output, "%ld, %ld\n", x, (time - rec->overhead) / n);
}


void recorder_free (recorder *rec) {
  fclose(rec->output);
  free(rec);
}
