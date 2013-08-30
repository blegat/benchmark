AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = file
file_SOURCES = file.c
file_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

GRAPHS = open.csv write.csv read.csv close.csv
PROG   = file
TMP    = tmp.dat


include ../lib/lib.mk

