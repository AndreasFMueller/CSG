#
# Makefile.am -- example: PDE
#
# (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
#
noinst_HEADERS = parameters.h 

noinst_LTLIBRARIES = libpde3.la

libpde3_la_SOURCES = 

noinst_PROGRAMS = pde3

pde3_SOURCES = pde3.cpp
pde3_LDADD = $(top_builddir)/lib/libcsg.la libpde3.la

test:	pde3
	time ./pde3 -XINFAS -d -x > pde3.off

