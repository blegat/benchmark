OBJ    = tab.o
GRAPHS = tab-lig.csv tab-col.csv
PROG   = tab
TMP    = tmp.dat
PERFS  = colonne.txt ligne.txt
EVENTS = cache-references,cache-misses

include ../lib/lib.mk

$(PERFS): $(PROGS)
	perf stat -e $(EVENTS) -o colonne.txt ./$(PROG) --colonne
	perf stat -e $(EVENTS) -o ligne.txt ./$(PROG) --ligne
