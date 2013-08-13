#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>

#include "benchmark.h"

#define MAX_SIZE 0x100000 // 1 MB

int main (int argc, char *argv[])  {
  timer *t = timer_alloc();
  recorder *shmget_rec = recorder_alloc("shmget.csv");
  recorder *shmat_rec = recorder_alloc("shmat.csv");
  recorder *shmdt_rec = recorder_alloc("shmdt.csv");

  size_t size;
  int err;
  key_t key=1252;
  for (size = getpagesize(); size <= MAX_SIZE; size *= 2) {
    key++;
    // BEGIN
    start_timer(t);
    int shm_id = shmget(key, size, IPC_CREAT | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    write_record(shmget_rec, size, stop_timer(t));
    if (shm_id == -1) {
      perror("shmget");
      exit(EXIT_FAILURE);
    }

    start_timer(t);
    void * addr = shmat(shm_id, NULL, 0);
    write_record(shmat_rec, size, stop_timer(t));
    if (addr == (void *) -1) {
      perror("shmat");
      exit(EXIT_FAILURE);
    }

    start_timer(t);
    err = shmdt(addr);
    write_record(shmdt_rec, size, stop_timer(t));
    if (err == -1) {
      perror("shmdt");
      exit(EXIT_FAILURE);
    }
    // END
  }

  recorder_free(shmget_rec);
  recorder_free(shmat_rec);
  recorder_free(shmdt_rec);
  timer_free(t);

  return EXIT_SUCCESS;
}
