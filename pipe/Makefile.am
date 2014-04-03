AM_CFLAGS = -I$(top_srcdir)/lib -BM_USE_CLOCK_GETTIME_RT @AM_CFLAGS@
bin_PROGRAMS = pipe
pipe_SOURCES = pipe.c
pipe_LDFLAGS = -lpthread
pipe_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

GRAPHS = pipe.csv
PROG   = pipe

include ../lib/lib.mk
