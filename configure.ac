#
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.57)
AC_REVISION([1.0])
AC_INIT([csg], [1.0], [andreas.mueller@hsr.ch])
AC_CONFIG_SRCDIR([common.h])
AC_CANONICAL_TARGET([])
AM_INIT_AUTOMAKE([-Wall])
AC_CONFIG_HEADERS(config.h)

# some configuration directives

# Checks for programs
AC_PROG_CXX
AC_PROG_CC
AM_PROG_AR
LT_INIT
AC_PROG_LIBTOOL

# Checks for header files
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h stdio.h unistd.h math.h errno.h string.h syslog.h sys/time.h])

# Check for libraries
AC_CHECK_LIB([gmp], [__gmpz_init])
AC_CHECK_LIB([mpfr], [mpfr_init])
AC_CHECK_LIB([CGAL], [_ZN4CGAL12warning_failEPKcS1_iS1_])

# set certain flags that are mandatory
CXXFLAGS="${CXXFLAGS} -frounding-math"
CFLAGS="${CFLAGS} -frounding-math"

# create files
AC_CONFIG_FILES(Makefile)
AC_OUTPUT
