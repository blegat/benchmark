AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = textbin
textbin_SOURCES = textbin.c textbinutil.c textbinutil.h
textbin_LDADD = $(top_builddir)/lib/libbenchmark.a \
                $(top_builddir)/lib/libcopy.a \
				$(AM_LDFLAGS)

PROG   = textbin
GRAPHS = text_write.csv text_read.csv bin_write.csv bin_read.csv

include ../lib/lib.mk

deps_template=template.sh template.c template.h start.c textbintemplate.c

textbinutil.c: ${deps_template}
	./template.sh

textbinutil.h: ${deps_template}
	./template.sh

# Ugly hack to make the generation of textbinutil.h a dependency on the generation of textbin.c on first build
# TODO Make textbinutil.h a dependency to textbin.o then renames textbintemplate.c to textbin.c and remove
#      cp textbintemplate.c textbin.c
#      from template.sh

textbin.c: ${deps_template}
	./template.sh
