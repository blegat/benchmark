AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@

# the library names to build (note we are building static libs only)
lib_LIBRARIES = libbenchmark.a libcopy.a

# where to install the headers on the system
libbenchmark_adir = $(includedir)/benchmark

# the list of header files that belong to the library (to be installed later)
libbenchmark_a_HEADERS =    \
                        benchmark.h

# the sources to add to the library and to add to the source distribution
libbenchmark_a_SOURCES =    \
                        $(libbenchmark_a_HEADERS) \
                        benchmark.c

libcopy_adir = $(includedir)/copy
libcopy_a_HEADERS = copy.h
libcopy_a_SOURCES = $(libcp_a_HEADERS) \
				  copy.c
