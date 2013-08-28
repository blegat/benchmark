# TODO mesure function call
# TODO memcpy taille de la zone mémoire, alignement 32 Bits (remove packed)
# TODO matrice ligne par ligne colonne par colonne
# TODO pipe, en fonction de la taille (père fils)

DEPS   += ../lib/benchmark.h
OBJ    += ../lib/benchmark.o
CC      = gcc

OS := $(shell uname)
ifeq ($(OS),Darwin)
CFLAGS += -I. -I../lib/ -g
else
CFLAGS += -I. -I../lib/ -g -lrt
endif

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

valgrind: $(PROG)
	valgrind --leak-check=full ./$(PROG)

$(GRAPHS): $(PROG)
	./$(PROG)

show: $(GRAPHS)
	gnuplot -p $(PROG).gpi

export: $(GRAPHS)
	gnuplot -p -e "set terminal png size 800,600 enhanced font 'Helvetica,12';\
	  set output '$(PROG).png'" $(PROG).gpi

.PHONY: run show export clean default mrproper

clean:
	$(RM) $(PROG) $(GRAPHS) $(OBJ) $(TMP)

mrproper: clean
	$(RM) *.png
