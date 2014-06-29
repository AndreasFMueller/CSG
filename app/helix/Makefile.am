#
# Makefile.am -- example: Helix
#
# (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
#
noinst_HEADERS = 

noinst_LTLIBRARIES = libhelix.la

libhelix_la_SOURCES = 

noinst_PROGRAMS = helix

helix_SOURCES = helix.cpp
helix_LDADD = $(top_builddir)/lib/libcsg.la libhelix.la

test:	helix
	time ./helix -d -f > helix.off

