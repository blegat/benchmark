AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = alloc
alloc_SOURCES = alloc.c
alloc_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

PROG   = alloc
GRAPHS = heap.csv stack.csv

include ../lib/lib.mk
