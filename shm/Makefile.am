AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = shm
shm_SOURCES = shm.c
shm_LDFLAGS = -lpthread
shm_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

GRAPHS = shm.cvs heap.cvs
PROG   = shm

include ../lib/lib.mk

