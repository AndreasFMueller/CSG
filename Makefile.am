#
# Makefile.am -- tools to prepare mathematical models for 3D printing 
#
# (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
#
lib_LTLIBRARIES = libcsg.la

libcsg_la_SOURCES = common.cpp						\
	CartesianCoordinatesSurface.cpp					\
	PolarCoordinatesSurface.cpp					\
	IcosahedralSphere.cpp						\
	SphericalSphere.cpp						\
	SphericalSurface.cpp						\
	Arrow.cpp

include_HEADERS = common.h						\
	CartesianCoordinatesSurface.h					\
	PolarCoordinatesSurface.h					\
	IcosahedralSphere.h						\
	SphericalSphere.h						\
	SphericalSurface.h						\
	Arrow.h

noinst_PROGRAMS = example1
example1_SOURCES = example1.cpp
example1_LDADD = libcsg.la

test:	example1
	time ./example1 -d -r 10 -n 10 > example1.off
