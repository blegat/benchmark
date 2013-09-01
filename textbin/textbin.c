/**
 * \file textbin.c
 * \brief Compare la sauvegarde de nombres en binaire et en ascii
 *
 * Écris et lis des nombres sauvegardés en binaire puis en ascii.
 * La sauvegarde en binaire se fait avec `read` et `write` et celle
 * en ascii se fait avec `fprintf` et `fscanf` auquel on
 * a désactivé le buffer de `stdio` pour que ce soit plus équitable.
 *
 * Problème
 * * `0xff` partout donne 0 pour un nombre à virgule flottante
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

#define N 100000
#define F "tmp.dat"

/**
 * \brief Renvoie un nombre de taille `size`
 *
 * Renvoie un pointeur vers un nombre de taille `size` alloué
 * dynamiquement avec tous les bits mis à 1.
 *
 * Il ne faut donc pas oublier de le libérer avec `free`.
 *
 * \param size la taille du nombre en byte.
 */
char *get_num (int size) {
  int i;
  char *s = (char *) malloc(sizeof(char) * size);
  if (s == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  for (i = 0; i < size; i++) {
    s[i] = 0xff;
  }
  return s;
}

/**
 * \brief Benchmark l'écriture et lecture en binaire d'un nombre
 *        de taille `size`
 *
 * \param t `timer` à utiliser pour mesurer le temps
 * \param read_rec `recorder` pour la lecture
 * \param write_rec `recorder` pour l'écriture
 * \param size taille du nombre à écrire
 */
void bin (timer *t, recorder *read_rec, recorder *write_rec,
    int size) {
  int err, i, len;

  rm(F);

  int fdout = open(F, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
  if (fdout == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  int fdin = open(F, O_RDONLY);
  if (fdin == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  fsync(fdin);
  fsync(fdout);

  char *s = get_num(size);

  start_timer(t);
  for (i = 0; i < N; i++) {
    len = write(fdout, (void *) s, size);
    if (len == -1) {
      perror("write");
      exit(EXIT_FAILURE);
    }
  }
  fsync(fdout);
  write_record(write_rec, len, stop_timer(t));

  start_timer(t);
  for (i = 0; i < N; i++) {
    len = read(fdin, (void *) s, size);
    if (len == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }
  }
  fsync(fdin);
  write_record(read_rec, len, stop_timer(t));

  free(s);

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

  rm(F);
}


/**
 * \brief Benchmark l'écriture et lecture en ascii d'un nombre
 *        de taille `size`
 *
 * Mesure les performances d'écriture, lecture de `N` nombre
 * en ascii avec `fprintf`, `fscanf`.
 *
 * \param t `timer` à utiliser pour mesurer le temps
 * \param read_rec `recorder` pour la lecture
 * \param write_rec `recorder` pour l'écriture
 * \param size taille du nombre à écrire
 * \param format format du nombre pour `printf`, `scanf`.
 *        Il vaut mieux ajouter un charactère de séparation également
 *        e.g. "%d "
 */
void text (timer *t, recorder *read_rec, recorder *write_rec,
    int size, char *format) {
  int err, i;

  rm(F);

  FILE *fout = fopen(F, "w");
  if (fout == NULL) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  FILE *fin = fopen(F, "r");
  if (fin == NULL) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  fflush(fin);
  fflush(fout);

  char *s = get_num(size);

  /**
   * Pour être équitable avec `write` et `read`,
   * on désactive le buffer de `stdio`
   */
  setvbuf(fin, NULL, _IONBF, 0);
  setvbuf(fout, NULL, _IONBF, 0);

  start_timer(t);
  for (i = 0; i < N; i++) {
    err = fprintf(fout, format, *s);
    if (err < 0) {
      perror("fprintf");
      exit(EXIT_FAILURE);
    }
  }
  fflush(fout); // Flush data to the kernel
  int fdout = fileno(fout);
  if (fdout == -1) {
    perror("fileno");
    exit(EXIT_FAILURE);
  }
  fsync(fdout);
  write_record(write_rec, size, stop_timer(t));

  start_timer(t);
  for (i = 0; i < N; i++) {
    err = fscanf(fin, format, s);
    if (err < 0) {
      perror("fscanf");
      exit(EXIT_FAILURE);
    }
  }
  int fdin = fileno(fin);
  if (fdin == -1) {
    perror("fileno");
    exit(EXIT_FAILURE);
  }
  fsync(fdin); // Write data to the disk
  write_record(read_rec, size, stop_timer(t));

  free(s);

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

  rm(F);
}

int main (int argc, char *argv[])  {
  timer *t = timer_alloc();
  recorder *text_read_rec = recorder_alloc("text_read.csv");
  recorder *text_write_rec = recorder_alloc("text_write.csv");
  recorder *bin_read_rec = recorder_alloc("bin_read.csv");
  recorder *bin_write_rec = recorder_alloc("bin_write.csv");

  bin(t, bin_write_rec, bin_read_rec, sizeof(short));
  text(t, text_write_rec, text_read_rec, sizeof(short), "%hd ");
  bin(t, bin_write_rec, bin_read_rec, sizeof(int));
  text(t, text_write_rec, text_read_rec, sizeof(int), "%d ");
  bin(t, bin_write_rec, bin_read_rec, sizeof(long int));
  text(t, text_write_rec, text_read_rec, sizeof(long int), "%ld ");
  bin(t, bin_write_rec, bin_read_rec, sizeof(long long int));
  text(t, text_write_rec, text_read_rec, sizeof(long long int), "%lld ");

  bin(t, bin_write_rec, bin_read_rec, sizeof(float));
  text(t, text_write_rec, text_read_rec, sizeof(float), "%f ");
  bin(t, bin_write_rec, bin_read_rec, sizeof(double));
  text(t, text_write_rec, text_read_rec, sizeof(double), "%lf ");
  bin(t, bin_write_rec, bin_read_rec, sizeof(long double));
  text(t, text_write_rec, text_read_rec, sizeof(long double), "%Lf ");


  timer_free(t);

  return EXIT_SUCCESS;
}
