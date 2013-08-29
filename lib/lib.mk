#//! \file lib.mk
#//! \brief Rassemble les règles communes à tous les `Makefile`s
#//!
#//! À inclure depuis le `Makefile` d'un benchmark en spécifiant les constantes
#//! suivantes
#//! * `GRAPHS`, les `.csv` générés;
#//! * `PROG`, le nom du programme;
#//! * `bin_PROGRAMS`, le nom des programmes nécessaires pour exécuter `$(PROG)`
#//!            (en plus de `$(PROG)`);
#//! * `TMP`, les fichiers temporaires générés par le programme
#//!          qu'il faut supprimer avec `make clean`.

OPEN=xdg-open

$(GRAPHS): $(bin_PROGRAMS)
	./$(PROG)

show-graph: $(GRAPHS)
	gnuplot -p $(PROG).gpi

$(PROG).png: $(GRAPHS) $(PROG).gpi
	gnuplot -p -e "set terminal png size 800,600 enhanced font 'Helvetica,12';\
	  set output '$(PROG).png'" $(PROG).gpi

index.html: $(PROG).png ../lib/gen_html.sh README.html $(PERFS)
	../lib/gen_html.sh $(PROG) $(PERFS) > index.html

show-html: index.html
	$(OPEN) index.html &

mrproper: clean
	$(RM) $(GRAPHS) $(PERFS) $(PROG).png index.html $(TMP)

.PHONY: mrproper valgrind
