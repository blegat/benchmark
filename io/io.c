/**
 * \file io.c
 * \brief Compare les performances de read/write avec fgets/fputs
 *
 * Compare aussi l'impact de la `kernel buffer cache` et du buffer au
 * niveau de `stdio`.
 * C'est inspiré du chapitre 13 du livre "The Linux programming interface"
 * par *Michael Kerrisk*.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#ifndef O_DIRECT
#warning O_DIRECT not defined on this platform
#define O_DIRECT 0
#endif

#include "benchmark.h"
#include "copy.h"

#define FILE_SIZE 0x40000 // 256 KiB

#define BUF_SIZE 0x10000 // 64 KiB
#define MAX_LEN 0X100000 // 1 MiB
#define PERF_LEN 512

#define IN "tmpin.dat"
#define OUT "tmpout.dat"

int main (int argc, char *argv[])  {
  /**
   * Si il y a des arguments,
   * `--sys_sync`, `--sys_nosync`, `--sys_direct`, `--std_buf` et `--std_nobuf`
   * font uniquement le test correspondant avec une taille de `PERF_LEN`.
   */
  if (argc > 1) {
    // perf
    if (strncmp(argv[1], "--sys_sync", 11) == 0) {
      read_write(NULL, NULL, IN, OUT, FILE_SIZE, PERF_LEN, O_SYNC);
    } else if (strncmp(argv[1], "--sys_nosync", 13) == 0) {
      read_write(NULL, NULL, IN, OUT, FILE_SIZE, PERF_LEN, 0);
    } else if (strncmp(argv[1], "--sys_direct", 13) == 0) {
      read_write(NULL, NULL, IN, OUT, FILE_SIZE, PERF_LEN, O_SYNC | O_DIRECT);
    } else if (strncmp(argv[1], "--std_buf", 10) == 0) {
      gets_puts(NULL, NULL, IN, OUT, FILE_SIZE, PERF_LEN, 1, BUF_SIZE);
    } else if (strncmp(argv[1], "--std_nobuf", 12) == 0) {
      gets_puts(NULL, NULL, IN, OUT, FILE_SIZE, PERF_LEN, 0, 0);
    }
  } else {
    // benchmark
    timer *t = timer_alloc();
    size_t len = 0;

    recorder *sys_sync_rec = recorder_alloc("sys_sync.csv");
    recorder *sys_nosync_rec = recorder_alloc("sys_nosync.csv");
    recorder *sys_direct_rec = recorder_alloc("sys_direct.csv");
    /**
     * On commence avec 512 pour `sys_sync` parce qu'en dessous
     * très lent et que de toute façon `sys_direct` ne sais pas aller
     * en dessous et le reste du graphe serait moins lisible même
     * en `log y`.
     */
    for (len = 512; len <= MAX_LEN; len *= 0x2) {
      read_write(t, sys_sync_rec, IN, OUT, FILE_SIZE, len, O_SYNC);
    }
    for (len = 2; len <= MAX_LEN; len *= 0x2) {
      read_write(t, sys_nosync_rec, IN, OUT, FILE_SIZE, len, 0);
    }
    /**
     * Pour `sys_sync`, `len` doit être un multiple de la taille d'un block
     * du file system. 512 est ok selon le *Kerrisk* cité plus haut.
     */
    for (len = 512; len <= MAX_LEN; len *= 2) {
      read_write(t, sys_direct_rec, IN, OUT, FILE_SIZE, len, O_SYNC | O_DIRECT);
    }
    recorder_free(sys_sync_rec);
    recorder_free(sys_nosync_rec);
    recorder_free(sys_direct_rec);

    recorder *std_buf_rec = recorder_alloc("std_buf.csv");
    recorder *std_nobuf_rec = recorder_alloc("std_nobuf.csv");
    for (len = 2; len <= MAX_LEN; len *= 0x2) {
      gets_puts(t, std_buf_rec, IN, OUT, FILE_SIZE, len, 1, BUF_SIZE);
    }
    for (len = 2; len <= MAX_LEN; len *= 0x2) {
      gets_puts(t, std_nobuf_rec, IN, OUT, FILE_SIZE, len, 0, 0);
    }
    recorder_free(std_buf_rec);
    recorder_free(std_nobuf_rec);

    timer_free(t);
  }

  return EXIT_SUCCESS;
}
