AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = thread
thread_SOURCES = thread.c
thread_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

GRAPHS = create.csv join.csv
PROG   = thread
CFLAGS = -lpthread


include ../lib/lib.mk

