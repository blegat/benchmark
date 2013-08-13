#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <unistd.h>
#include <pthread.h>

#include "benchmark.h"

#define N 100

int main (int argc, char *argv[])  {
  timer *t = timer_alloc();
  recorder *parent_rec = recorder_alloc("parent.csv");
  recorder *child_rec = recorder_alloc("child.csv");

  pid_t pid;
  int status, i;

  for (i = 0; i < N; i++) {
    update_overhead(); // does not change anything
    start_timer(t);
    pid = fork();

    if (pid == -1) {
      // erreur à l'exécution de fork
      perror("fork");
      return EXIT_FAILURE;
    }
    // pas d'erreur
    if (pid == 0) {
      // processus fils
      write_record(child_rec, i, stop_timer(t));
      return EXIT_SUCCESS;
    }
    else {
      // processus père
      write_record(parent_rec, i, stop_timer(t));
      pid = wait(&status);
      if (pid == -1) {
        perror("wait");
        return EXIT_FAILURE;
      }
    }
    printf("%d\n", pid);
  }

  recorder_free(child_rec);
  recorder_free(parent_rec);
  timer_free(t);

  return EXIT_SUCCESS;
}
