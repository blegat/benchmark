OBJ    = memfork.o
GRAPHS = memfor-beforefork.csv memfor-aftfork.csv memfor-aftmodif.csv
PROG   = memfork
TMP    = tmp.dat
PERFS  = before.txt after.txt
CFLAGS = -BM_USE_CLOCK_GETTIME
EVENTS = dTLB-loads,dTLB-load-misses,dTLB-stores,dTLB-store-misses,dTLB-prefetches,dTLB-prefetch-misses,iTLB-loads,iTLB-load-misses

include ../lib/lib.mk

$(PERFS): $(PROGS)
	perf stat -e $(EVENTS) -o before.txt ./$(PROG) --before
	perf stat -e $(EVENTS) -o after.txt ./$(PROG) --after
