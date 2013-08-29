# initial information about the project
AC_INIT([benchmark],[0.0],[olivier.bonaventure@uclouvain.be])

# automake initialization (mandatory) including a check for automake API version >= 1.9
AM_INIT_AUTOMAKE([1.9])

LT_INIT

AC_CONFIG_MACRO_DIR([m4])

# check for C++ preprocessor and compiler
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LIBTOOL

# use the C++ compiler for the following checks
AC_LANG([C])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([error.h])
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([time.h])
AC_CHECK_HEADERS([unistd.h])
AC_CHECK_HEADERS([sys/stat.h])
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([sys/times.h])
AC_CHECK_HEADERS([sys/types.h])
AC_PATH_PROG([GNUPLOT], [gnuplot], [notfound])
AC_PATH_PROG([PERF], [perf], [notfound])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# distribute additional compiler and linker flags among Makefiles
# --> set and change these variables instead of CXXFLAGS or LDFLAGS (for user only)
AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_LDFLAGS])

# files to generate via autotools (prepare .am or .in source files)
#AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([io/Makefile])
AC_CONFIG_FILES([shell/Makefile])
AC_CONFIG_FILES([lib/Makefile])

# finally this generates the Makefiles etc. for the build
AC_OUTPUT
