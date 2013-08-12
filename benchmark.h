/*  _____ _
 * |_   _(_)_ __ ___   ___ _ __
 *   | | | | '_ ` _ \ / _ \ '__|
 *   | | | | | | | | |  __/ |
 *   |_| |_|_| |_| |_|\___|_|
 */

typedef struct timer timer;
struct timer;

/*
 * Alloue un nouveau timer.
 * Différents timers sont nécessaires dans le cas de multithreading
 * ou tout simplement dans le cas de plusieurs mesures de temps simultanées.
 */
timer *timer_alloc ();

void start_timer (timer *t);

long int stop_timer (timer *t);

void *timer_free (timer *t);

/*  ____                        _
 * |  _ \ ___  ___ ___  _ __ __| | ___ _ __
 * | |_) / _ \/ __/ _ \| '__/ _` |/ _ \ '__|
 * |  _ <  __/ (_| (_) | | | (_| |  __/ |
 * |_| \_\___|\___\___/|_|  \__,_|\___|_|
 */

long int overhead = -1;
long int get_overhead () {
  if (-1 == overhead) {
    timer *t = timer_alloc();
    start_timer(t);
    overhead = stop_timer(t);
    timer_free(t);
    printf("overhead: %ld\n", overhead);
  }
  return overhead;
}

typedef struct recorder recorder;
struct recorder {
  FILE *output;
  long int overhead;
};

recorder *recorder_alloc (char *filename) {
  recorder *rec = (recorder *) malloc(sizeof(recorder));
  if (rec == NULL) {
    perror("malloc");
    return NULL;
  }
  rec->output = fopen(filename, "w");
  if (rec->output == NULL) {
    perror("fopen");
    free(rec);
    return NULL;
  }
  rec->overhead = get_overhead();
  return rec;
}

void write_record (recorder *rec, long int x, long int time) {
  fprintf(rec->output, "%ld, %ld\n", x, time - rec->overhead);
}

void recorder_free (recorder *rec) {
  fclose(rec->output);
  free(rec);
}
