/**
 * \brief Benchmark l'écriture et lecture en binaire d'un nombre
 *        de taille `size`
 *
 * \param t `timer` à utiliser pour mesurer le temps
 * \param read_rec `recorder` pour la lecture
 * \param write_rec `recorder` pour l'écriture
 * \param size taille du nombre à écrire
 */
void bin_POST (timer *t, recorder *read_rec, recorder *write_rec, TYPE value) {
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

  start_timer(t);
  for (i = 0; i < N; i++) {
    len = write(fdout, (void *) &value, sizeof(TYPE));
    if (len == -1) {
      perror("write");
      exit(EXIT_FAILURE);
    }
  }
  fsync(fdout);
  write_record(write_rec, len, stop_timer(t));

  start_timer(t);
  for (i = 0; i < N; i++) {
    len = read(fdin, (void *) &value, sizeof(TYPE));
    if (len == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }
  }
  fsync(fdin);
  write_record(read_rec, len, stop_timer(t));

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
void text_POST (timer *t, recorder *read_rec, recorder *write_rec, TYPE value) {
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

  /**
   * Pour être équitable avec `write` et `read`,
   * on désactive le buffer de `stdio`
   */
  setvbuf(fin, NULL, _IONBF, 0);
  setvbuf(fout, NULL, _IONBF, 0);

  start_timer(t);
  for (i = 0; i < N; i++) {
    err = fprintf(fout, FORMAT, value);
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
  write_record(write_rec, sizeof(TYPE), stop_timer(t));

  start_timer(t);
  for (i = 0; i < N; i++) {
    err = fscanf(fin, FORMAT, &value);
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
  write_record(read_rec, sizeof(TYPE), stop_timer(t));

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
