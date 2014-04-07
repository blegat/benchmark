/**
 * \file mmap.c
 * \brief Compare les performances d'une copie avec et sans `mmap`
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "benchmark.h"
#include "copy.h"

#define FILE_SIZE 0x1000000 // 1 MiB
#define MAX_SIZE  0x1000000 // 1 MiB

#define IN "tmpin.dat"
#define OUT "tmpout.dat"

int main (int argc, char *argv[]) {
  int err;

  timer *t = timer_alloc();
  recorder *rw_rec = recorder_alloc("rw.csv");
  recorder *mmap_rec = recorder_alloc("mmap.csv");

  size_t len = 0;
  int page_size = getpagesize();

  for (len = 0x40; len <= MAX_SIZE; len *= 2) {
    read_write(t, rw_rec, IN, OUT, FILE_SIZE, len, 0);
  }

  for (len = page_size; len <= MAX_SIZE; len *= 2) {
    mmap_munmap(t, mmap_rec, IN, OUT, FILE_SIZE, len);
  }

  recorder_free(rw_rec);
  recorder_free(mmap_rec);

  return EXIT_SUCCESS;
}
