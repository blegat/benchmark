#ifndef __BENCHMARK_H__
#define __BENCHMARK_H__
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

void timer_free (timer *t);

/*  ____                        _
 * |  _ \ ___  ___ ___  _ __ __| | ___ _ __
 * | |_) / _ \/ __/ _ \| '__/ _` |/ _ \ '__|
 * |  _ <  __/ (_| (_) | | | (_| |  __/ |
 * |_| \_\___|\___\___/|_|  \__,_|\___|_|
 */

void update_overhead ();
long int get_overhead ();

typedef struct recorder recorder;
struct recorder;

recorder *recorder_alloc (char *filename);

void write_record (recorder *rec, long int x, long int time);
void write_record_n (recorder *rec, long int x, long int time, long n);

void recorder_free (recorder *rec);

#endif
