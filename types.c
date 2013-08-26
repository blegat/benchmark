#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "benchmark.h"

#define N 29999

int primeInt (int nMax, timer* t, recorder* r) {
	int count, i = 3, c;
	
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

int main (int argc, char *argv[]) {
	
	timer *t = timer_alloc();
	recorder *int_rec = recorder_alloc("int.csv");
	recorder *long_rec = recorder_alloc("long.csv");
	recorder *float_rec = recorder_alloc("float.csv");
	
	primeInt(N,t,int_rec);
	primeLong(N,t,long_rec);
	primeFloat(N,t,float_rec);
	
	recorder_free(int_rec);
	recorder_free(long_rec);
	recorder_free(float_rec);
	timer_free(t);
	
	return 0;
}
