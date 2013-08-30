AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = pipe
pipe_SOURCES = pipe.c
pipe_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

GRAPHS = pipe.csv
PROG   = pipe
CFLAGS = -lpthread -BM_USE_CLOCK_GETTIME_RT


include ../lib/lib.mk

