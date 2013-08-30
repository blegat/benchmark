AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = memfork
memfork_SOURCES = memfork.c
memfork_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

GRAPHS = memfor-beforefork.csv memfor-aftfork.csv memfor-aftmodif.csv
PROG   = memfork
TMP    = tmp.dat
PERFS  = before.txt after.txt
EVENTS = dTLB-loads,dTLB-load-misses,dTLB-stores,dTLB-store-misses,dTLB-prefetches,dTLB-prefetch-misses,iTLB-loads,iTLB-load-misses

#CFLAGS = -BM_USE_CLOCK_GETTIME

include ../lib/lib.mk

$(PERFS): $(PROGS)
	perf stat -e $(EVENTS) -o before.txt ./$(PROG) --before
	perf stat -e $(EVENTS) -o after.txt ./$(PROG) --after
