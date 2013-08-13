/**************************************
 * benchmark.c
 *
 * Des fonctions simples de benchmark.
 **************************************/

#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <unistd.h>
#include "benchmark.h"

/*  _____ _
 * |_   _(_)_ __ ___   ___ _ __
 *   | | | | '_ ` _ \ / _ \ '__|
 *   | | | | | | | | |  __/ |
 *   |_| |_|_| |_| |_|\___|_|
 */

struct timer {
  struct timeval start;
};

timer *timer_alloc () {
  timer *t = (timer *) malloc(sizeof(timer));
  if (t == NULL) {
    perror("malloc");
    exit(-1);
  }
  return t;
}

void start_timer (timer *t) {
  gettimeofday(&t->start, NULL);
}
long int stop_timer (timer *t) {
  struct timeval end;
  gettimeofday(&end, NULL);
  return end.tv_sec * 1000000 + end.tv_usec -
    t->start.tv_sec * 1000000 + t->start.tv_usec;
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
}
long int get_overhead () {
  if (-1 == overhead) {
    update_overhead();
  }
  return overhead;
}

typedef struct recorder recorder;
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
  fprintf(rec->output, "%ld, %ld\n", x, time - rec->overhead);
}

void recorder_free (recorder *rec) {
  fclose(rec->output);
  free(rec);
}
