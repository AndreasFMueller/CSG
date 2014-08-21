#
# Makefile.am -- example: PDE
#
# (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
#
noinst_HEADERS = parameters.h solution.h characteristics.h axes.h curves.h \
	support.h

noinst_LTLIBRARIES = libpde1.la

libpde1_la_SOURCES = solution.cpp characteristics.cpp axes.cpp curves.cpp \
	support.cpp

noinst_PROGRAMS = pde1

pde1_SOURCES = pde1.cpp
pde1_LDADD = $(top_builddir)/lib/libcsg.la libpde1.la

test:	pde1
	time ./pde1 -XINFAS -d -x > pde1.off

