#include alloc.mk
#include thread.mk
#include fork.mk
#include file.mk
#include mmap.mk
include shm.mk

CC      = gcc
CFLAGS += -I.

# Lorsqu'on fait `make`, c'est la première règle qui est exécutée
# donc dans ce cas-ci, c'est sa dependance.
default: show

# $@ c'est la règle, ici le nom de l'objet comme `benchmark.o`
# $< c'est $< c'est le premier élément de la liste des dépendances, ici
#    c'est la source comme `benchmark.c`
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROG): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(GRAPHS): $(PROG)
	./$(PROG)

show: $(GRAPHS)
	gnuplot -p $(PROG).gpi

export: $(GRAPHS)
	gnuplot -p -e "set terminal png size 800,600 enhanced font 'Helvetica,12';\
	  set output '$(PROG).png'" $(PROG).gpi

.PHONY: run show export clean default

clean:
	$(RM) $(PROG) $(GRAPHS) $(OBJ) $(TMP)
