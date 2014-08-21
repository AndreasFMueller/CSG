#
# Makefile.am -- example: PDE
#
# (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
#
noinst_HEADERS = parameters.h solution.h characteristics.h axes.h \
	initialcurve.h support.h

noinst_LTLIBRARIES = libpde2.la

libpde2_la_SOURCES = solution.cpp characteristics.cpp axes.cpp \
	initialcurve.cpp support.cpp

noinst_PROGRAMS = pde2

pde2_SOURCES = pde2.cpp
pde2_LDADD = $(top_builddir)/lib/libcsg.la libpde2.la

test:	pde2
	time ./pde2 -d -s 32 -c 50 > pde2.off

