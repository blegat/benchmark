#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <float.h>

#include "benchmark.h"
#include "textbinutil.h"

#define N 100

int main (int argc, char *argv[])  {
  timer *t = timer_alloc();
  recorder *text_read_rec = recorder_alloc("text_read.csv");
  recorder *text_write_rec = recorder_alloc("text_write.csv");
  recorder *bin_read_rec = recorder_alloc("bin_read.csv");
  recorder *bin_write_rec = recorder_alloc("bin_write.csv");

  short s = SHRT_MAX;
  bin_short(t, bin_write_rec, bin_read_rec, s);
  text_short(t, text_write_rec, text_read_rec, s);
  int i = INT_MAX;
  bin_int(t, bin_write_rec, bin_read_rec, i);
  text_int(t, text_write_rec, text_read_rec, i);
  long int li = LONG_MAX;
  bin_long_int(t, bin_write_rec, bin_read_rec, li);
  text_long_int(t, text_write_rec, text_read_rec, li);
  long long int lli = LLONG_MAX;
  bin_long_long_int(t, bin_write_rec, bin_read_rec, lli);
  text_long_long_int(t, text_write_rec, text_read_rec, lli);

  float f = FLT_MAX;
  bin_float(t, bin_write_rec, bin_read_rec, f);
  text_float(t, text_write_rec, text_read_rec, f);
  double d = DBL_MAX;
  bin_double(t, bin_write_rec, bin_read_rec, d);
  text_double(t, text_write_rec, text_read_rec, d);
  long double ld = LDBL_MAX;
  bin_long_double(t, bin_write_rec, bin_read_rec, ld);
  text_long_double(t, text_write_rec, text_read_rec, ld);

  timer_free(t);

  return EXIT_SUCCESS;
}
