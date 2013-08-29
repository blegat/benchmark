AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = shell shell-program

shell_SOURCES = shell.c
shell_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

shell_program_SOURCES = shell-program.c
shell_program_LDADD =  $(AM_LDFLAGS)

PROG   = shell
GRAPHS = shell-bash.csv shell-prog.csv
TMP    = tmp.dat

include ../lib/lib.mk
