AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = mmap
mmap_SOURCES = mmap.c
mmap_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

GRAPHS = open.csv close.csv mmap.csv munmap.csv
PROG   = mmap
TMP    = tmp.dat

include ../lib/lib.mk

