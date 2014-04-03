# initial information about the project
AC_INIT([benchmark],[0.0],[olivier.bonaventure@uclouvain.be])

# automake initialization (mandatory) including a check
# for automake API version >= 1.9
# foreign is for ignoring that there is no NEWS, README, ...
AM_INIT_AUTOMAKE([1.9 -Wall foreign])

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
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([alloc/Makefile])
AC_CONFIG_FILES([io/Makefile])
AC_CONFIG_FILES([lib/Makefile])
AC_CONFIG_FILES([shell/Makefile])
AC_CONFIG_FILES([argfct/Makefile])
AC_CONFIG_FILES([memfork/Makefile])
AC_CONFIG_FILES([mutsem/Makefile])
AC_CONFIG_FILES([amdahl/Makefile])
AC_CONFIG_FILES([calloc/Makefile])
AC_CONFIG_FILES([file/Makefile])
AC_CONFIG_FILES([fork/Makefile])
AC_CONFIG_FILES([mmap/Makefile])
AC_CONFIG_FILES([pipe/Makefile])
AC_CONFIG_FILES([shm/Makefile])
AC_CONFIG_FILES([tab/Makefile])
AC_CONFIG_FILES([textbin/Makefile])
AC_CONFIG_FILES([thread/Makefile])
AC_CONFIG_FILES([types/Makefile])

AM_CONDITIONAL(OS_IS_MAC, [test $(uname -s) = Darwin])

# finally this generates the Makefiles etc. for the build
AC_OUTPUT
