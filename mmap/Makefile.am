AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = mmap
mmap_SOURCES = mmap.c
mmap_LDADD = $(top_builddir)/lib/libbenchmark.a \
             $(top_builddir)/lib/libcopy.a \
			 $(AM_LDFLAGS)

GRAPHS = rw.csv mmap.csv
PROG   = mmap
TMP    = tmpin.dat tmpout.dat

include ../lib/lib.mk

