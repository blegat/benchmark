/**
 * \file template.c
 * \brief Compare la sauvegarde de nombres en binaire et en ascii
 *
 * Écris et lis des nombres sauvegardés en binaire puis en ascii.
 * La sauvegarde en binaire se fait avec `read` et `write` et celle
 * en ascii se fait avec `fprintf` et `fscanf` auquel on
 * a désactivé le buffer de `stdio` pour que ce soit plus équitable.
 *
 * Problème
 * * `0xff` partout donne 0 pour un nombre à virgule flottante
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <float.h>

#include "benchmark.h"
#include "copy.h"
#include "textbinutil.h"

#define N 100
#define F "tmp.dat"
