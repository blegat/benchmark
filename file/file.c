#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "benchmark.h"

#define MAX_SIZE 0x100000 // 1 MB

int main (int argc, char *argv[])  {
  timer *t = timer_alloc();
  recorder *open_rec = recorder_alloc("open.csv");
  recorder *write_rec = recorder_alloc("write.csv");
  recorder *read_rec = recorder_alloc("read.csv");
  recorder *close_rec = recorder_alloc("close.csv");

  int err, size;
  ssize_t len;
  start_timer(t);
  int fd = open("tmp.dat", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
  write_record(open_rec, 1, stop_timer(t));
  if(fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  char *s = (char *) malloc((MAX_SIZE + 1) * sizeof(char));
  memset(s, 0, MAX_SIZE + 1);
  for (size = 1; size <= MAX_SIZE; size *= 2) {
    start_timer(t);
    len = write(fd, (void *) s, size);
    write_record(write_rec, size, stop_timer(t));
    if (len == -1) {
      perror("write");
      return EXIT_FAILURE;
    }
  }
  int file_size = size - 1; // e.g. 1 + 2 + 4 + 8 + 16 = 32 - 1
  start_timer(t);
  err = close(fd);
  write_record(close_rec, 1, stop_timer(t));
  if (err == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  // BEGIN
  start_timer(t);
  fd = open("tmp.dat", O_RDONLY);
  write_record(open_rec, file_size, stop_timer(t));
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  for (size = 1; size <= MAX_SIZE; size *= 2) {
    start_timer(t);
    len = read(fd, (void *) s, size);
    write_record(read_rec, size, stop_timer(t));
    if (len == -1) {
      perror("read");
      return EXIT_FAILURE;
    }
  }
  start_timer(t);
  err = close(fd);
  write_record(close_rec, file_size, stop_timer(t));
  if(err == -1){
    perror("close");
    exit(EXIT_FAILURE);
  }
  // END

  free(s);
  recorder_free(open_rec);
  recorder_free(write_rec);
  recorder_free(read_rec);
  recorder_free(close_rec);
  timer_free(t);

  return EXIT_SUCCESS;
}
