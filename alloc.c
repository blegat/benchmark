#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <unistd.h>

#include <benchmark.h>

#define MAX_SIZE 0x10000000 // 1 MB
#define MAX_ALLOCS 100 // largement assez

int main (int argc, char *argv[]) {
  int i;
  int size;
  void *p;
  timer *t = timer_alloc();

  if (argc < 2) {
    fprintf(stderr, "Missing argument\n");
    return -1;
  }

  // malloc_free
  if (strncmp("--free", argv[1], 7) == 0) {
    printf("free\n");
    recorder *malloc_rec = recorder_alloc("malloc.csv");
    recorder *calloc_rec = recorder_alloc("calloc.csv");
    recorder *free_rec = recorder_alloc("free.csv");
    for (size = 1; size <= MAX_SIZE; size *= 2) {
      start_timer(t);
      p = malloc(size);
      write_record(malloc_rec, size, stop_timer(t));
      if (p == NULL) {
        perror("malloc");
        return -1;
      }
      printf("malloc\t%p\t%p\n", p, sbrk(0));

      start_timer(t);
      free(p);
      write_record(free_rec, size, stop_timer(t));
      printf("free\t%p\t%p\n", p, sbrk(0));

      start_timer(t);
      p = calloc(size, 1);
      write_record(calloc_rec, size, stop_timer(t));
      if (p == NULL) {
        perror("calloc");
        return -1;
      }
      printf("calloc\t%p\t%p\n", p, sbrk(0));

      start_timer(t);
      free(p);
      write_record(free_rec, size, stop_timer(t));
      printf("free\t%p\t%p\n", p, sbrk(0));
    }
    recorder_free(malloc_rec);
    recorder_free(free_rec);
  } else if (strncmp("--nofree", argv[1], 9) == 0) {
    printf("nofree\n");
    // malloc_nofree
    void **allocs = (void **) malloc(sizeof(void*) * MAX_ALLOCS);
    recorder *malloc_nofree_rec = recorder_alloc("malloc_nofree.csv");
    for (size = 1, i = 0; size <= MAX_SIZE; size *= 2, i++) {
      start_timer(t);
      allocs[i] = malloc(size);
      write_record(malloc_nofree_rec, size, stop_timer(t));
      if (allocs[i] == NULL) {
        perror("malloc");
        return -1;
      }
      printf("malloc\t%p\t%p\n", allocs[i], sbrk(0));
    }
    for (size = 1, i = 0; size <= MAX_SIZE; i++, size *= 2) {
      free(allocs[i]);
    }
    recorder_free(malloc_nofree_rec);
  } else if (strncmp("--brk", argv[1], 6) == 0) {
    // brk/sbrk
    recorder *sbrk_rec = recorder_alloc("sbrk.csv");
    recorder *brk_rec = recorder_alloc("brk.csv");
    for (size = 1; size <= MAX_SIZE; size *= 2) {
      start_timer(t);
      p = sbrk(size); // alloc
      write_record(sbrk_rec, size, stop_timer(t));
      if (p == NULL) {
        perror("sbrk");
      }
      start_timer(t);
      int err = brk(p); // free
      write_record(brk_rec, size, stop_timer(t));
      if (err == -1) {
        perror("brk");
      }
    }
    recorder_free(sbrk_rec);
    recorder_free(brk_rec);
  } else {
    fprintf(stderr, "Unknown option\n");
    return -1;
  }

  printf("%p\n", sbrk(0));

  timer_free(t);
  return EXIT_SUCCESS;
}
