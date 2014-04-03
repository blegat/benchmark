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
Commencez par vous assurez que vous avez les dépendances nécessaires

    $ sudo apt-get install build-essential dh-autoreconf gnuplot

Il est nécessaire d'avoir un gnuplot récent (4.2 est trop vieux, 4.6 marche bien).
Si vous êtes sur une des machines des salles, il vous faudra donc l'installer from source (voir plus loin).

Pour générer les `Makefile` nécessaires à la compilation à partir
des `Makefile.am`, exécutez

    $ git clone https://github.com/blegat/benchmark.git
    $ cd benchmark
    $ ./bootstrap.sh

Ça va non seulement générer un `Makefile` à la racine qui compile
les librairies et tous les benchmarks mais aussi un `Makefile` dans
le dossier `lib` qui compile les librairies et un dossier dans
chaque dossier de benchmark qui peut compiler le benchmark mais aussi
l'exécuter et générer et afficher une page `html` à l'aide `make show-html`.

Par exemple, pour essayer le benchmark `io`, faite suivre ce `./bootstrap.sh`
par

    $ cd io
    $ make show-html

### Utiliser gnuplot from source

Assurez-vous d'installer `libgd2-xpm-dev` pour avoir le support de png dans gnuplot

    $ sudo apt-get install libgd2-xpm-dev

puis téléchargez et installer le
(`./configure` devrait trouver `libgd` et activer le support de png pour gnuplot)

    $ wget http://sourceforge.net/projects/gnuplot/files/gnuplot/4.6.5/gnuplot-4.6.5.tar.gz
    $ tar xzvf gnuplot-4.6.5.tar.gz
    $ cd gnuplot-4.6.5
    $ ./configure
    $ make

Si vous n'avez les droits root sur la machine
(c'est le cas si vous êtes sur une machine des salles), vous ne pourrez pas l'installer,
et il faudra spécifier au benchmark d'utiliser votre gnuplot compilé dans `~/src/gnuplot-4.6.5/src`.
Pour cela, faites

    $ PATH=<path/to/gnuplot-4.6.5>/src:${PATH} ./bootstrap.sh

à la place de `./bootstrap.sh` dans le dossier benchmark.

Si vous avez les droits root, faites simplement

    $ sudo make install

dans le dossier gnuplot.


### Troobleshooting

Si vous avez l'erreur "required file `./ltmain.sh`" en faisant `./configure` ou `./bootstrap.sh`,
assurez vous que votre dossier `gnuplot-4.6.5` n'est pas dans `benchmark` et vice versa.
