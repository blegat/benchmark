AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = amdahl
amdahl_SOURCES = amdahl.c
amdahl_LDFLAGS = -lpthread
amdahl_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

GRAPHS = thread.csv proc.csv
PROG   = amdahl
PERFS  = amdahl.txt

include ../lib/lib.mk

$(PERFS): $(PROGS)
	perf stat -o amdahl.txt ./$(PROG)
