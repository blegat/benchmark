/**
 * \file io.c
 * \brief Compare les performances ce read/write avec fgets/fputs
 *
 * Compare aussi l'impact de la `kernel buffer cache` et du buffer au
 * niveau de `stdio`.
 * C'est inspiré du chapitre 13 du livre "The Linux programming interface"
 * par Michael Kerrisk
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <error.h>

#include "benchmark.h"

#define SIZE 0x40000 // 256 KiB
#define LEN 0x1000 // 4 KiB
#define BUF_SIZE 0x10000 // 64 KiB
#define MAX_LEN 0X100000 // 1 MiB
#define PERF_LEN 512

#define IN "tmpin.dat"
#define OUT "tmpout.dat"

void create_file (char *in) {
  FILE *f = fopen(in, "w");
  char *s = (char*) malloc(sizeof(char) * (LEN + 1));
  memset(s, '\0' + 1, LEN);
  s[LEN] = '\0';
  int i;
  for (i = 0; i < SIZE;) {
    i += LEN;
    if (i > SIZE) {
      s[LEN - (i - SIZE)] = '\0';
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

void read_write (timer *t, recorder *rec,
    int len, int flags) {
  // for O_DIRECT, len must be a multiple of the logical block
  // size of the file system or EINVAL will be received from
  // read/write
  int err;

  printf("%d\n", len);
  rm(IN);
  rm(OUT);

  create_file(IN);

  int fdin = open(IN, O_RDONLY|flags);
  if(fdin == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  int fdout = open(OUT, O_WRONLY|O_CREAT|O_TRUNC|flags, S_IRUSR|S_IWUSR);
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
  int i = 0;
  for (i = 0; i < SIZE; i += len_read) {
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
}


void gets_puts (timer *t, recorder *rec,
    int len, int has_buf, size_t buf_size) {
  // for O_DIRECT, len must be a multiple of the logical block
  // size of the file system or EINVAL will be received from
  // read/write
  int err;

  printf("%d\n", len);
  rm(IN);
  rm(OUT);

  create_file(IN);

  FILE *fin = fopen(IN, "r");
  if(fin == NULL) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  FILE *fout = fopen(OUT, "w");
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
  int i = 0;
  for (i = 0; i < SIZE; i += len) {
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
}

int main (int argc, char *argv[])  {
  /**
   * Si il y a des arguments, --sys ne fait que les appels systèmes
   * et --std ne fait que std et les autres options font que le programme
   * ne fait rien
   */
  if (argc > 1) {
    // perf
    if (strncmp(argv[1], "--sys_sync", 11) == 0) {
      read_write(NULL, NULL, PERF_LEN, O_SYNC);
    } else if (strncmp(argv[1], "--sys_nosync", 13) == 0) {
      read_write(NULL, NULL, PERF_LEN, 0);
    } else if (strncmp(argv[1], "--sys_direct", 13) == 0) {
      read_write(NULL, NULL, PERF_LEN, O_SYNC | O_DIRECT);
    } else if (strncmp(argv[1], "--std_buf", 10) == 0) {
      gets_puts(NULL, NULL, PERF_LEN, 1, BUF_SIZE);
    } else if (strncmp(argv[1], "--std_nobuf", 12) == 0) {
      gets_puts(NULL, NULL, PERF_LEN, 0, 0);
    }
  } else {
    // benchmark
    timer *t = timer_alloc();
    int len = 0;

    recorder *sys_sync_rec = recorder_alloc("sys_sync.csv");
    recorder *sys_nosync_rec = recorder_alloc("sys_nosync.csv");
    recorder *sys_direct_rec = recorder_alloc("sys_direct.csv");
    // En dessous de 512, c'est très lent et le graphe est moins lisible
    for (len = 512; len <= MAX_LEN; len *= 0x2) {
      read_write(t, sys_sync_rec, len, O_SYNC);
    }
    for (len = 2; len <= MAX_LEN; len *= 0x2) {
      read_write(t, sys_nosync_rec, len, 0);
    }
    // On doit être un multiple d'un block du file system donc il faut
    // commencer avec 512
    for (len = 512; len <= MAX_LEN; len *= 2) {
      read_write(t, sys_direct_rec, len, O_SYNC | O_DIRECT);
    }
    recorder_free(sys_sync_rec);
    recorder_free(sys_nosync_rec);
    recorder_free(sys_direct_rec);

    recorder *std_buf_rec = recorder_alloc("std_buf.csv");
    recorder *std_nobuf_rec = recorder_alloc("std_nobuf.csv");
    for (len = 2; len <= MAX_LEN; len *= 0x2) {
      gets_puts(t, std_buf_rec, len, 1, BUF_SIZE);
    }
    for (len = 2; len <= MAX_LEN; len *= 0x2) {
      gets_puts(t, std_nobuf_rec, len, 0, 0);
    }
    recorder_free(std_buf_rec);
    recorder_free(std_nobuf_rec);

    timer_free(t);
  }

  return EXIT_SUCCESS;
}
