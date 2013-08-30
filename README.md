# Benchmark

## Introduction
Ce projet contient des benchmarks des appels systèmes Unix qui sont
intéressants d'un point de vue didactique.
Ils sont utilisés par le cours [SINF1252](http://sinf1252.info.ucl.ac.be/)
donné à l'UCL par le professeur Olivier Bonaventure.

Ils ont été écrits par Maxime De Mol, Nicolas Houtain et Benoît Legat
avec l'aide d'Oliver Bonaventure.

Ils utilisent l'outil
[perf](https://perf.wiki.kernel.org/index.php/Main_Page)
pour analyser différents conteurs ainsi que
[gnuplot](http://www.gnuplot.info/)
pour afficher les benchmarks.

Le projet est constitué
* d'un dossier `lib` contenant des petites librairies
  statiques développées pour le projet ainsi que divers scripts;
* ainsi qu'un dossier par benchmark.
  Le résultat du benchmark est affiché dans une page `html`
  qu'on peut obtenir en lançant `make show-html` dans le dossier correspondant.

## Documentation
La documentation se trouve
[ici](http://blegat.github.io/benchmark/index.html)
mais elle peut être générée à l'aide de
[Doxygen](http://www.stack.nl/~dimitri/doxygen/)
ainsi

    $ doxygen Doxyfile
Il suffit alors d'ouvrir `doc/html/index.html` pour voir la documentation.

## Compilation
Pour générer les `Makefile` nécessaires à la compilation à partir
des `Makefile.am`, exécutez

    $ ./bootstrap.sh

Ça va non seulement générer un `Makefile` à la racine qui compile
les librairies et tous les benchmarks mais aussi un `Makefile` dans
le dossier `lib` qui compile les librairies et un dossier dans
chaque dossier de benchmark qui peut compiler le benchmark mais aussi
l'exécuter et générer et afficher une page `html` à l'aide `make show-html`.

Par exemple, pour essayer le benchmark `io`, faite suivre ce `./bootstrap.sh`
par

    $ make
    $ cd io
    $ make show-html
