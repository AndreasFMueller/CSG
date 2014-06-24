#
# Makefile.am -- tools to prepare mathematical models for 3D printing 
#
# (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
#
lib_LTLIBRARIES = libcsg.la

libcsg_la_SOURCES = common.cpp timekeeper.cpp debug.cpp			\
	Surface.cpp							\
	Cartesian.cpp							\
	Polar.cpp							\
	IcosahedralSphere.cpp						\
	SphericalSphere.cpp						\
	SphericalSurface.cpp						\
	Arrow.cpp							\
	Curve.cpp							\
	Line.cpp							\
	Box.cpp

include_HEADERS = common.h timekeeper.h	debug.h				\
	Surface.h							\
	Cartesian.h							\
	Polar.h								\
	IcosahedralSphere.h						\
	SphericalSphere.h						\
	SphericalSurface.h						\
	Arrow.h								\
	Curve.h								\
	Line.h								\
	Box.h

noinst_PROGRAMS = example1 example2 example3 example4 example5 example6 \
	example7

example1_SOURCES = example1.cpp
example1_LDADD = libcsg.la

example2_SOURCES = example2.cpp
example2_LDADD = libcsg.la

example3_SOURCES = example3.cpp
example3_LDADD = libcsg.la

example4_SOURCES = example4.cpp
example4_LDADD = libcsg.la

example5_SOURCES = example5.cpp
example5_LDADD = libcsg.la

example6_SOURCES = example6.cpp
example6_LDADD = libcsg.la

example7_SOURCES = example7.cpp
example7_LDADD = libcsg.la

test:	example1
	time ./example1 -d -r 10 -n 2 > example1.off

test2:	example2
	time ./example2 -d -n 50 > example2.off

test3:	example3
	time ./example3 -d -n 200 -s 32 > example3.off

test4:	example4
	time ./example4 -d -s 32 > example4.off

test5:	example5
	time ./example5 -d -s 32 > example5.off

test6:	example6
	time ./example6 -d > example6.off

test7:	example7
	time ./example7 -d > example7.off
