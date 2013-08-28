/***********************************************************
 * types.c
 *
 * Comparaison de vitesse de calcul avec differents types.
 ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "benchmark.h"

#define N 999

/*
 *  _____       _
 * |_   _|     | |
 *   | |  _ __ | |_ ___  __ _  ___ _ __
 *   | | | '_ \| __/ _ \/ _` |/ _ \ '__|
 *  _| |_| | | | ||  __/ (_| |  __/ |
 * |_____|_| |_|\__\___|\__, |\___|_|
 *                       __/ |
 *	   				    |___/
 */

/**
 * \brief `primeInt` cherche les `nMax` premiers nombres premiers
 *
 * \param nMax Défini combien de nombres premiers on cherche
 *
 * \param t Le temps dans lequel on stoque le temps de début
 *
 * \param r Sauvegarde le temps dans un fichier .csv
 *
 * Cherche les `nMax` premiers nombres premiers par la methode de brute force.
 * Utilise exclusivement des int pour le calcul. A la fin du calcul sauvegarde
 * le temps écoulé dans un fichier .csv en utilisant un `recorder` .
 */


int primeInt (int nMax, timer* t, recorder* r) {
	int count, i = 3, c;
	
	start_timer(t);
	// on lance le timer
	
	for (count = 2; count <= nMax; ) {
		for (c = 2; c <= i ; c++) {
			if (i%c == 0) {
				break;
				// si le modulo == 0 => pas nombre premier => break
			}
		}
		if (c == i) {
			count ++;
			if (count % 50 == 0){
				write_record_n(r,count,stop_timer(t),nMax);
				// on sauvegarde le temps pris pour trouver le `n ième` n.p.
			}
		}
		i++;
	}
	return 0;
}

/*
 *  _                         _                     _       _
 * | |                       | |                   (_)     | |
 * | |     ___  _ __   __ _  | | ___  _ __   __ _   _ _ __ | |_
 * | |    / _ \| '_ \ / _` | | |/ _ \| '_ \ / _` | | | '_ \| __|
 * | |___| (_) | | | | (_| | | | (_) | | | | (_| | | | | | | |_
 * |______\___/|_| |_|\__, | |_|\___/|_| |_|\__, | |_|_| |_|\__|
 *                     __/ |                 __/ |
 *                    |___/                 |___/
 */

/**
 * \brief `primeLong` cherche les `nMax` premiers nombres premiers
 *
 * \param nMax Défini combien de nombres premiers on cherche
 *
 * \param t Le temps dans lequel on stoque le temps de début
 *
 * \param r Sauvegarde le temps dans un fichier .csv
 *
 * Cherche les `nMax` premiers nombres premiers par la methode de brute force.
 * Utilise exclusivement des long long int pour le calcul. A la fin du calcul
 * sauvegarde le temps écoulé dans un fichier .csv en utilisant un `recorder` .
 */

int primeLong (long long int nMax, timer* t, recorder* r) {
	long long int count, i = 3, c;
	
	start_timer(t);
	
	for (count = 2; count <= nMax; ) {
		for (c = 2; c <= i ; c++) {
			if (i%c == 0) {
				break;
			}
		}
		if (c == i) {
			count ++;
			if (count % 50 == 0){
				write_record_n(r,count,stop_timer(t),nMax);
			}
		}
		i++;
	}
	return 0;
}

/*
 *  ______ _             _
 * |  ____| |           | |
 * | |__  | | ___   __ _| |_
 * |  __| | |/ _ \ / _` | __|
 * | |    | | (_) | (_| | |_
 * |_|    |_|\___/ \__,_|\__|
 */

/**
 * \brief `primeFloat` cherche les `nMax` premiers nombres premiers
 *
 * \param nMax Défini combien de nombres premiers on cherche
 *
 * \param t Le temps dans lequel on stoque le temps de début
 *
 * \param r Sauvegarde le temps dans un fichier .csv
 *
 * Cherche les `nMax` premiers nombres premiers par la methode de brute force.
 * Utilise exclusivement des float pour le calcul. A la fin du calcul
 * sauvegarde le temps écoulé dans un fichier .csv en utilisant un `recorder` .
 */


int primeFloat (int nMax, timer* t, recorder* r) {
	float count, i = 3, c;
	
	start_timer(t);
	
	for (count = 2; count <= nMax; ) {
		for (c = 2; c <= i ; c++) {
			if (fmod(i,c) == 0.0) {
				break;
			}
		}
		if (c == i) {
			count ++;
			if (fmod(count,50.0) == 0.0){
				write_record_n(r,count,stop_timer(t),nMax);
			}
		}
		i++;
	}
	return 0;
}

/*
 *  __  __       _
 * |  \/  |     (_)
 * | \  / | __ _ _ _ __
 * | |\/| |/ _` | | '_ \
 * | |  | | (_| | | | | |
 * |_|  |_|\__,_|_|_| |_|
 */

/**
 * \brief Enregistre les temps de calcul nécessaire à trouver des nombres premiers
 *
 * On calcul les `N` premiers nombres premiers par la méthode de brute force en
 * utilisant 3 types différents: `int` ,`long long int` et `float` . A chaque fois
 * les temps de calcul sont enregistrés dans des fichier .csv et afficher sous
 * forme de graphe afin de permettre une lecture et comparaison facile de ceus là.
 */

int main (int argc, char *argv[]) {
	
	timer *t = timer_alloc();
	recorder *int_rec = recorder_alloc("int.csv");
	recorder *long_rec = recorder_alloc("long.csv");
	recorder *float_rec = recorder_alloc("float.csv");
	// on init tous les `recorders` et le timer
	
	primeInt(N,t,int_rec);
	primeLong(N,t,long_rec);
	primeFloat(N,t,float_rec);
	// on lance les tests
	
	recorder_free(int_rec);
	recorder_free(long_rec);
	recorder_free(float_rec);
	timer_free(t);
	// on free toutes nos structures
	
	return 0;
}
