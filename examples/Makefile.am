#
# Makefile.am -- tools to prepare mathematical models for 3D printing 
#
# (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
#

noinst_PROGRAMS = example1 example2 example3 example4 example5 example6

example1_SOURCES = example1.cpp
example1_LDADD = $(top_builddir)/lib/libcsg.la

example2_SOURCES = example2.cpp
example2_LDADD = $(top_builddir)/lib/libcsg.la

example3_SOURCES = example3.cpp
example3_LDADD = $(top_builddir)/lib/libcsg.la

example4_SOURCES = example4.cpp
example4_LDADD = $(top_builddir)/lib/libcsg.la

example5_SOURCES = example5.cpp
example5_LDADD = $(top_builddir)/lib/libcsg.la

example6_SOURCES = example6.cpp
example6_LDADD = $(top_builddir)/lib/libcsg.la

test:	test1 test2 test3 test4 test5 test6

test1:	example1
	time ./example1 -d -r 10 -n 2 > example1.off

test2:	example2
	time ./example2 -d -n 50 > example2.off

test3:	example3
	time ./example3 -d -n 200 -s 32 > example3.off

test4:	example4
	time ./example4 -d -s 32 > example4.off

test5:	example5
	time ./example5 -d > example5.off

test6:	example6
	time ./example6 -d > example6.off

