AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = argfct
argfct_SOURCES = argfct.c
argfct_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

PROG   = argfct
GRAPHS = argfct-val.csv argfct-pt.csv
TMP    = tmp.dat

#CFLAGS = -BM_USE_TIMES

include ../lib/lib.mk
