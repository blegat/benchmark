#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "benchmark.h"

#define MAX_SIZE 0x100000 // 1 MB

int main (int argc, char *argv[])  {
  timer *t = timer_alloc();
  recorder *open_rec = recorder_alloc("open.csv");
  recorder *close_rec = recorder_alloc("close.csv");
  recorder *mmap_rec = recorder_alloc("mmap.csv");
  recorder *munmap_rec = recorder_alloc("munmap.csv");

  int err, size, fd;
  char dummy = 0, *mapping = NULL;
  ssize_t len;
  for (size = getpagesize(); size <= MAX_SIZE; size *= 2) {
    // set the size
    fd = open("tmp.dat", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    if (fd == -1) {
      perror("open");
      exit(EXIT_FAILURE);
    }
    if (lseek(fd, size - 1, SEEK_SET) == -1) {
      perror("lseek");
      exit(EXIT_FAILURE);
    }
    if (write(fd, (void *) &dummy, 1) == -1) {
      perror("write");
      return EXIT_FAILURE;
    }
    err = close(fd);

    // open
    start_timer(t);
    fd = open("tmp.dat", O_RDONLY);
    write_record(open_rec, size, stop_timer(t));
    if (fd == -1) {
      perror("open");
      exit(EXIT_FAILURE);
    }

    // BEGIN
    // mmap
    start_timer(t);
    mapping = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
    write_record(mmap_rec, size, stop_timer(t));
    if (fd == -1) {
      perror("mmap");
      exit(EXIT_FAILURE);
    }

    // munmap
    start_timer(t);
    err = munmap(mapping, size);
    write_record(munmap_rec, size, stop_timer(t));
    if(err == -1){
      perror("munmap");
      exit(EXIT_FAILURE);
    }
    // END

    // close
    start_timer(t);
    err = close(fd);
    write_record(close_rec, size, stop_timer(t));
    if(err == -1){
      perror("close");
      exit(EXIT_FAILURE);
    }

  }

  recorder_free(open_rec);
  recorder_free(close_rec);
  recorder_free(mmap_rec);
  recorder_free(munmap_rec);
  timer_free(t);

  return EXIT_SUCCESS;
}
