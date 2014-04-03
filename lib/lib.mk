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
#//!
#//! Les règles suivantes sont ajoutées à tous les benchmarks
#//! * `show-plot` montre les résultats du benchmark à l'aide de `gnuplot`;
#//! * `show` montre les résultats du benchmark, éventuellement de `perf`
#//!    et affiche les commentaires en ouvrant un page web en local dans votre
#//!    navigateur par défaut;
#//! * `clean` supprime tous vos fichiers intermédiaire de compilation;
#//! * `mrproper` les supprime aussi mais supprime aussi tous les résultats
#//!    générés.

if OS_IS_MAC
  OPEN=open
else
  OPEN=xdg-open
endif

$(GRAPHS): $(bin_PROGRAMS)
	./$(PROG)

show-plot: $(GRAPHS) $(PROG).gpi
	$(GNUPLOT) -p $(PROG).gpi

$(PROG).png: $(GRAPHS) $(PROG).gpi
	$(GNUPLOT) -p -e "set terminal png size 800,600 enhanced font 'Helvetica,12';\
	  set output '$(PROG).png'" $(PROG).gpi

index.html: $(PROG).png ../lib/gen_html.sh README.html $(PERFS)
	../lib/gen_html.sh $(PROG) $(PERFS) > index.html

show: index.html
	$(OPEN) index.html &

mrproper: clean
	$(RM) $(GRAPHS) $(PERFS) $(PROG).png index.html $(TMP)

.PHONY: mrproper show show-plot
