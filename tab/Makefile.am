AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = tab
tab_SOURCES = tab.c
tab_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

GRAPHS = tab-lig.csv tab-col.csv
PROG   = tab
TMP    = tmp.dat
PERFS  = colonne.txt ligne.txt
EVENTS = cache-references,cache-misses

include ../lib/lib.mk

$(PERFS): $(PROGS)
	perf stat -e $(EVENTS) -o colonne.txt ./$(PROG) --colonne
	perf stat -e $(EVENTS) -o ligne.txt ./$(PROG) --ligne
