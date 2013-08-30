#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "benchmark.h"
#include "copy.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

// There is not much interest in changing the default
// since this is not benchmarked
#define CREATE_BUF_SIZE 0x1000 // 4 KiB

void create_file (char *in, size_t file_size) {
  FILE *f = fopen(in, "w");
  char *s = (char*) malloc(sizeof(char) * (CREATE_BUF_SIZE + 1));
  memset(s, '\0' + 1, CREATE_BUF_SIZE);
  s[CREATE_BUF_SIZE] = '\0';
  off_t i;
  for (i = 0; i < file_size;) {
    i += CREATE_BUF_SIZE;
    if (i > file_size) {
      s[CREATE_BUF_SIZE - (i - file_size)] = '\0';
    }
    fputs(s, f);
  }
  fclose(f);
}

void rm (char *fname) {
  int err;
  if (access(fname, F_OK) != -1) {
    err = unlink(fname);
    if (err == -1) {
      perror("unlink");
      exit(EXIT_FAILURE);
    }
  }
}

void read_write (timer *t, recorder *rec, char *in, char *out,
    size_t file_size, size_t len, int flags) {
  // for O_DIRECT, len must be a multiple of the logical block
  // size of the file system or EINVAL will be received from
  // read/write
  int err;

  printf("0x%lx\t0x%lx\n", len, file_size);
  rm(in);
  rm(out);

  create_file(in, file_size);

  int fdin = open(in, O_RDONLY|flags);
  if(fdin == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  int fdout = open(out, O_WRONLY|O_CREAT|O_TRUNC|flags, S_IRUSR|S_IWUSR);
  if(fdout == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  fsync(fdin); // Clear kernel buffer cache (actually, just syncing, no clear)
  fsync(fdout); // Clear kernel buffer cache

  //char *s = (char *) malloc(len * sizeof(char));
  // necessary for O_DIRECT
  char *s = NULL;
  err = posix_memalign((void **) &s, 512, len);
  if (err != 0) {
    error(0, err, "posix_memalign");
    exit(EXIT_FAILURE);
  }

  int len_read = 0, len_written = 0;
  if (rec != NULL) {
    start_timer(t);
  }
  off_t i = 0;
  for (i = 0; i < file_size; i += len_read) {
    len_read = read(fdin, (void *) s, len);
    if (len_read == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }
    len_written = write(fdout, (void *) s, len_read);
    if (len_written == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }
  }
  fsync(fdin); // Sync data
  fsync(fdout); // Sync data
  if (rec != NULL) {
    write_record(rec, len, stop_timer(t));
  }

  err = close(fdin);
  if (err == -1){
    perror("close");
    exit(EXIT_FAILURE);
  }
  err = close(fdout);
  if (err == -1){
    perror("close");
    exit(EXIT_FAILURE);
  }

  rm(in);
  rm(out);
}


void gets_puts (timer *t, recorder *rec, char *in, char *out,
    size_t file_size, size_t len, int has_buf, size_t buf_size) {
  // for O_DIRECT, len must be a multiple of the logical block
  // size of the file system or EINVAL will be received from
  // read/write
  int err;

  printf("0x%lx\t0x%lx\n", len, file_size);
  rm(in);
  rm(out);

  create_file(in, file_size);

  FILE *fin = fopen(in, "r");
  if(fin == NULL) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  FILE *fout = fopen(out, "w");
  if(fout == NULL) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  fflush(fout); // Clear stdio buffer cache

  char *in_buf = NULL;
  char *out_buf = NULL;
  if (has_buf) {
    in_buf = (char*) malloc(sizeof(char) * buf_size);
    out_buf = (char*) malloc(sizeof(char) * buf_size);
  }
  setvbuf(fin, in_buf, has_buf ? _IOFBF : _IONBF, buf_size);
  setvbuf(fout, out_buf, has_buf ? _IOFBF : _IONBF, buf_size);

  //char *s = (char *) malloc(len * sizeof(char));
  // necessary for O_DIRECT
  char *s = malloc(sizeof(char) * len);
  if (s == NULL) {
    perror("posix_memalign");
    exit(EXIT_FAILURE);
  }

  if (rec != NULL) {
    start_timer(t);
  }
  off_t i = 0;
  for (i = 0; i < file_size; i += len) {
    s = fgets((void *) s, len, fin);
    if (s == NULL) {
      perror("fgets");
      exit(EXIT_FAILURE);
    }
    err = fputs((void *) s, fout);
    if (err == EOF) {
      perror("fputs");
      exit(EXIT_FAILURE);
    }
  }
  int fdin = fileno(fin);
  if (fdin == -1) {
    perror("fileno");
    exit(EXIT_FAILURE);
  }
  fsync(fdin); // Write data to the disk
  fflush(fout); // Flush data to the kernel
  int fdout = fileno(fout);
  if (fdout == -1) {
    perror("fileno");
    exit(EXIT_FAILURE);
  }
  fsync(fdout); // Write data to the disk
  if (rec != NULL) {
    write_record(rec, len, stop_timer(t));
  }

  err = fclose(fin);
  if (err == EOF){
    perror("fclose");
    exit(EXIT_FAILURE);
  }
  err = fclose(fout);
  if (err == EOF){
    perror("fclose");
    exit(EXIT_FAILURE);
  }

  rm(in);
  rm(out);
}

void mmap_munmap (timer *t, recorder *rec, char *in, char *out,
    size_t file_size, size_t len) {
  int err;
  char dummy = 0;

  printf("0x%lx\t0x%lx\n", len, file_size);
  rm(in);
  rm(out);

  create_file(in, file_size);

  int fdin = open(in, O_RDONLY);
  if (fdin == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  int fdout = open(out, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
  if (fdout == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  if (lseek(fdout, file_size - 1, SEEK_SET) == -1) {
    perror("lseek");
    exit(EXIT_FAILURE);
  }
  if (write(fdout, &dummy, sizeof(char)) != 1) {
    perror("write");
    exit(EXIT_FAILURE);
  }

  fsync(fdin); // Clear kernel buffer cache (actually, just syncing, no clear)
  fsync(fdout); // Clear kernel buffer cache

  char *min = NULL, *mout = NULL;

  if (rec != NULL) {
    start_timer(t);
  }
  off_t i = 0;
  for (i = 0; i < file_size; i += len) {
    size_t len_mapped = MIN(len, file_size - i);
    // PRIVATE to avoid caching
    min = (char *) mmap(NULL, len_mapped, PROT_READ, MAP_SHARED,
        fdin, i);
    if (min == (char *) MAP_FAILED) {
      perror("mmap_in");
      exit(EXIT_FAILURE);
    }
    mout = (char *) mmap(NULL, len_mapped, PROT_READ | PROT_WRITE, MAP_SHARED,
        fdout, i);
    if (mout == (char *) MAP_FAILED) {
      perror("mmap_out");
      exit(EXIT_FAILURE);
    }

    memcpy(mout, min, len_mapped);

    err = munmap(min, len_mapped);
    if (err == -1) {
      perror("munmap");
      exit(EXIT_FAILURE);
    }
    err = munmap(mout, len_mapped);
    if (err == -1) {
      perror("munmap");
      exit(EXIT_FAILURE);
    }
  }
  fsync(fdin); // Sync data
  fsync(fdout); // Sync data
  if (rec != NULL) {
    write_record(rec, len, stop_timer(t));
  }

  err = close(fdin);
  if (err == -1){
    perror("close");
    exit(EXIT_FAILURE);
  }
  err = close(fdout);
  if (err == -1){
    perror("close");
    exit(EXIT_FAILURE);
  }

  rm(in);
  rm(out);
}
