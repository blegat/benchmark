OBJ    = memfork.o
GRAPHS = memfor-beforefork.csv memfor-aftfork.csv memfor-aftmodif.csv
PROG   = memfork
TMP    = tmp.dat
PERFS  = before.txt after.txt

include ../lib/lib.mk

$(PERFS): $(PROGS)
	perf stat -o before.txt ./$(PROG) --before
	perf stat -o after.txt ./$(PROG) --after
