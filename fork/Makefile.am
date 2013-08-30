AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = fork
fork_SOURCES = fork.c
fork_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

GRAPHS = parent.csv child.csv
PROG   = fork
TMP    = tmp.dat

include ../lib/lib.mk
