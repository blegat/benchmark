OBJ    = tab.o
GRAPHS = tab-lig.csv tab-col.csv
PROG   = tab
TMP    = tmp.dat
PERFS  = colonne.txt ligne.txt

include ../lib/lib.mk

$(PERFS): $(PROGS)
	perf stat -o colonne.txt ./$(PROG) --colonne
	perf stat -o ligne.txt ./$(PROG) --ligne
