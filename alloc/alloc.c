#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//#define BM_USE_CLOCK_GETTIME_RT
#include "benchmark.h"

#define MAX_SIZE 0x100000 // 1 MB
#define MAX_ALLOCS 100 // largement assez
#define N 100000
#define ALLOC_SIZE 1000000
#define N_HOLE 10000
#define SIZE_HOLE 0x1000

void stack () {
  char s[ALLOC_SIZE];
  s[ALLOC_SIZE / 2] = 0;
}

void heap () {
  char *s = (char *) malloc(sizeof(char) * ALLOC_SIZE);
  s[ALLOC_SIZE / 2] = 0;
  free(s);
}

int main (int argc, char *argv[]) {
  int i;
  int size;
  void *p;
  timer *t = timer_alloc();

  // brk/sbrk
  //update_overhead(); // needs to be done, remove it to see why
  recorder *stack_rec = recorder_alloc("stack.csv");
  recorder *heap_rec = recorder_alloc("heap.csv");

  start_timer(t);
  for (i = 0; i < N; i++) {
    stack();
  }
  write_record_n(stack_rec, ALLOC_SIZE, stop_timer(t), N);

  start_timer(t);
  for (i = 0; i < N; i++) {
    heap();
  }
  write_record_n(heap_rec, ALLOC_SIZE, stop_timer(t), N);

  /*recorder *malloc_rec = recorder_alloc("malloc.csv");
  recorder *calloc_rec = recorder_alloc("calloc.csv");
  recorder *free_rec = recorder_alloc("free.csv");
  for (size = 1; size <= MAX_SIZE; size *= 2) {
    start_timer(t);
    for (i = 0; i < N; i++) {
      p = malloc(size);
      free(p);
    }
    write_record_n(malloc_rec, size, stop_timer(t), N);*/
    /*if (p == NULL) {
      perror("malloc");
      return -1;
    }
    //printf("malloc\t%p\t%p\n", p, sbrk(0));

    start_timer(t);
    free(p);
    write_record(free_rec, size, stop_timer(t));
    //printf("free\t%p\t%p\n", p, sbrk(0));

    start_timer(t);
    p = calloc(size, 1);
    write_record(calloc_rec, size, stop_timer(t));
    if (p == NULL) {
      perror("calloc");
      return -1;
    }
    //printf("calloc\t%p\t%p\n", p, sbrk(0));

    start_timer(t);
    free(p);
    write_record(free_rec, size, stop_timer(t));*/
    //printf("free\t%p\t%p\n", p, sbrk(0));
  /*}
  recorder_free(malloc_rec);
  recorder_free(calloc_rec);
  recorder_free(free_rec);

  recorder *sbrk_rec = recorder_alloc("sbrk.csv");
  recorder *brk_rec = recorder_alloc("brk.csv");
  for (size = 1; size <= MAX_SIZE; size *= 2) {
    // BEGIN
    start_timer(t);
    for (i = 0; i < N; i++) {
      p = sbrk(size); // alloc
      brk(p); // free
    }
    write_record_n(sbrk_rec, size, stop_timer(t), N);*/
    /*if (p == NULL) {
      perror("sbrk");
    }
    start_timer(t);
    int err = brk(p); // free
    write_record(brk_rec, size, stop_timer(t));
    if (err == -1) {
      perror("brk");
    }*/
    // END
  /*}
  recorder_free(sbrk_rec);
  recorder_free(brk_rec);*/


  printf("%p\n", sbrk(0));

  timer_free(t);
  return EXIT_SUCCESS;
}
