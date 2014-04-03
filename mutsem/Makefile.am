AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = mutsem
mutsem_SOURCES = mutsem.c
mutsem_LDFLAGS = -lpthread
mutsem_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

GRAPHS = mutsem-mut.csv mutsem-sem.csv
PROG   = mutsem
TMP    = tmp.dat

include ../lib/lib.mk
