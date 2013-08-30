AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = calloc
calloc_SOURCES = calloc.c
calloc_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

GRAPHS = calloc.csv malloc.csv
PROG   = calloc
TMP    = tmp.dat

include ../lib/lib.mk
