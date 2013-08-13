CC     = gcc
CFLAGS = -I.
DEPS   = benchmark.h
OBJ    = benchmark.o alloc.o 

# Lorsqu'on fait `make`, c'est la première règle qui est exécutée
# donc dans ce cas-ci, c'est sa dependance.
default: show

# $@ c'est la règle, ici le nom de l'objet comme `benchmark.o`
# $< c'est $< c'est le premier élément de la liste des dépendances, ici
#    c'est la source comme `benchmark.c`
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

alloc: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

free.csv malloc.csv calloc.csv: alloc
	./alloc --free

brk.csv sbrk.csv: alloc
	./alloc --brk

show: free.csv malloc.csv calloc.csv brk.csv sbrk.csv
	gnuplot -p alloc.gnuplot

.PHONY: run show clean default

clean:
	$(RM) alloc *.csv *.o
