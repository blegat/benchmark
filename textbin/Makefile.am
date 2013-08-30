AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = textbin
textbin_SOURCES = textbin.c
textbin_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

PROG   = textbin
GRAPHS = text_write.csv text_read.csv bin_write.csv bin_read.csv

include ../lib/lib.mk
