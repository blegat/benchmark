#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "benchmark.h"

#define MAX_SIZE 0x1000 // 4 KB
#define N 200000
#define N_SYNC 20
//#define N_SYNC 100

int main (int argc, char *argv[])  {
  timer *t = timer_alloc();
  recorder *write_nosync_rec = recorder_alloc("write_nosync.csv");
  recorder *read_nosync_rec = recorder_alloc("read_nosync.csv");
  recorder *write_sync_rec = recorder_alloc("write_sync.csv");
  recorder *read_sync_rec = recorder_alloc("read_sync.csv");
  recorder *fputc_rec = recorder_alloc("fputc.csv");
  recorder *fgetc_rec = recorder_alloc("fgetc.csv");

  // TODO compare O_SYNC
  // TODO compare printf scanf, putc, ...
  int err, size, i, k;
  ssize_t len;
  char *s = (char *) malloc((MAX_SIZE + 1) * sizeof(char));
  memset(s, 0, MAX_SIZE + 1);

  recorder *write_rec = write_nosync_rec;
  recorder *read_rec = read_nosync_rec;
  int sync = 0, n = N;
  for (k = 0; k < 2; k++) {
    printf("%d\n", k);
    for (size = 1; size <= MAX_SIZE; size *= 2) {
      printf("%d\n", size);
      int fd = open("tmp.dat", O_RDWR|O_CREAT|O_TRUNC|sync, S_IRUSR|S_IWUSR);
      if(fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
      }

      start_timer(t);
      for (i = 0; i < n; i++) {
        len = write(fd, (void *) s, size);
      }
      write_record_n(write_rec, size, stop_timer(t), n);
      if (len == -1) {
        perror("write");
        return EXIT_FAILURE;
      }

      lseek(fd, 0, SEEK_SET);

      // FIXME need to clear cache

      start_timer(t);
      for (i = 0; i < n; i++) {
        len = read(fd, (void *) s, size);
      }
      write_record_n(read_rec, size, stop_timer(t), n);
      if (len == -1) {
        perror("read");
        return EXIT_FAILURE;
      }

      err = close(fd);
      if(err == -1){
        perror("close");
        exit(EXIT_FAILURE);
      }
    }
    write_rec = write_sync_rec;
    read_rec = read_sync_rec;
    sync = O_SYNC | O_DIRECT;
    n = N_SYNC;
  }

  recorder_free(write_nosync_rec);
  recorder_free(read_nosync_rec);
  recorder_free(write_sync_rec);
  recorder_free(read_sync_rec);

  /*for (size = 1; size <= MAX_SIZE; size *= 2) {
    FILE *f = fopen("tmp.dat", "w+");
    if (f == NULL) {
      perror("fopen");
      exit(EXIT_FAILURE);
    }

    start_timer(t);
    for (i = 0; i < N_STD; i++) {
      fputs(s, f);
    }
    write_record_n(fputs_rec, size, stop_timer(t), n);
    if (len == -1) {
      perror("write");
      return EXIT_FAILURE;
    }

    lseek(fd, 0, SEEK_SET);

    // FIXME need to clear cache

    start_timer(t);
    for (i = 0; i < N; i++) {
      len = read(fd, (void *) s, size);
    }
    write_record_n(read_rec, size, stop_timer(t), N);
    if (len == -1) {
      perror("read");
      return EXIT_FAILURE;
    }

    err = close(fd);
    if(err == -1){
      perror("close");
      exit(EXIT_FAILURE);
    }
  }


  recorder_free(fputc_rec);
  recorder_free(fgetc_rec);*/
  free(s);
  timer_free(t);

  return EXIT_SUCCESS;
}
