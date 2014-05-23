#
# Makefile.am -- tools to prepare mathematical models for 3D printing 
#
# (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
#
lib_LTLIBRARIES = libcsg.la

libcsg_la_SOURCES = common.cpp						\
	Surface.cpp							\
	Cartesian.cpp							\
	Polar.cpp							\
	IcosahedralSphere.cpp						\
	SphericalSphere.cpp						\
	SphericalSurface.cpp						\
	Arrow.cpp							\
	Curve.cpp

include_HEADERS = common.h						\
	Surface.h							\
	Cartesian.h							\
	Polar.h								\
	IcosahedralSphere.h						\
	SphericalSphere.h						\
	SphericalSurface.h						\
	Arrow.h								\
	Curve.h

noinst_PROGRAMS = example1
example1_SOURCES = example1.cpp
example1_LDADD = libcsg.la

test:	example1
	time ./example1 -d -r 10 -n 2 > example1.off

