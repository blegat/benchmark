#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "benchmark.h"

#define N 1000

void *thread(void * param) {
  sleep(1); // pas un `while (true)` comme ça le thread est pas "ready"
  pthread_exit(NULL);
}

int main (int argc, char *argv[])  {
  pthread_t threads[N];
  int err = 0, i;

  timer *t = timer_alloc();
  recorder *create_rec = recorder_alloc("create.csv");
  recorder *join_rec = recorder_alloc("join.csv");

  // BEGIN
  start_timer(t);
  for (i = 0; i < N; i++) {
    err = pthread_create(&threads[i], NULL, &thread, NULL);
    /*if (err != 0)
      error(1, err, "pthread_create");*/
  }
  write_record_n(create_rec, i, stop_timer(t), N);
  // END

  sleep(2); // Pour s'assurer que les pthread_join ne patientent pas

  // BEGIN
  start_timer(t);
  for (i = 0; i < N; i++) {
    err = pthread_join(threads[i], NULL);
    /*if (err != 0)
      error(1, err, "pthread_join");*/
  }
  write_record_n(join_rec, i, stop_timer(t), N);
  // END

  recorder_free(join_rec);
  recorder_free(create_rec);
  timer_free(t);

  return EXIT_SUCCESS;
}
