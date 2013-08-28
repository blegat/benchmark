# TODO mesure function call
# TODO memcpy taille de la zone mémoire, alignement 32 Bits (remove packed)
# TODO matrice ligne par ligne colonne par colonne
# TODO pipe, en fonction de la taille (père fils)

DEPS   += ../lib/benchmark.h
OBJ    += ../lib/benchmark.o
CC      = gcc

OS := $(shell uname)
CFLAGS += -I. -I../lib/ -g -Wall
ifeq ($(OS), Darwin)
else
CFLAGS += -lrt
endif

# Lorsqu'on fait `make`, c'est la première règle qui est exécutée
# donc dans ce cas-ci, c'est sa dependance.
default: show_html

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

show_graph: $(GRAPHS)
	gnuplot -p $(PROG).gpi

$(PROG).png: $(GRAPHS) $(PROG).gpi
	gnuplot -p -e "set terminal png size 800,600 enhanced font 'Helvetica,12';\
	  set output '$(PROG).png'" $(PROG).gpi

index.html: $(PROG).png ../lib/gen_html.sh README.html
	../lib/gen_html.sh $(PROG) > index.html

show_html: index.html
	xdg-open index.html &

.PHONY: run show clean default mrproper valgrind

clean:
	$(RM) $(PROG) $(GRAPHS) $(OBJ) $(TMP)

mrproper: clean
	$(RM) *.png index.html
