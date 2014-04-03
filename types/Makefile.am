AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = types
types_SOURCES = types.c
types_LDFLAGS = -lm
types_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

GRAPHS = int.csv float.csv long.csv
PROG   = types

include ../lib/lib.mk
