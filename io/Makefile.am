AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = io
io_SOURCES = io.c
io_LDADD = $(top_builddir)/lib/libbenchmark.a \
		   $(top_builddir)/lib/libcopy.a \
		   $(AM_LDFLAGS)

PROG   = io
GRAPHS = sys_sync.csv sys_nosync.csv sys_direct.csv std_buf.csv std_nobuf.csv
TMP    = tmpin.dat tmpout.dat
PERFS  = sys_sync.txt sys_nosync.txt sys_direct.txt std_buf.txt std_nobuf.txt

include ../lib/lib.mk

$(PERFS): $(bin_PROGRAMS)
	perf stat -o sys_sync.txt ./$(PROG) --sys_sync
	perf stat -o sys_nosync.txt ./$(PROG) --sys_nosync
	perf stat -o sys_direct.txt ./$(PROG) --sys_direct
	perf stat -o std_buf.txt ./$(PROG) --std_buf
	perf stat -o std_nobuf.txt ./$(PROG) --std_nobuf
