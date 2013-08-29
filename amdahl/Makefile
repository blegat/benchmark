OBJ    = amdahl.o
GRAPHS = thread.csv proc.csv
PROG   = amdahl
CFLAGS = -lpthread

include ../lib/lib.mk

$(PERFS): $(PROGS)
	perf stat -o amdahl.txt ./$(PROG)