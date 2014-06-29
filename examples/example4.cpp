/*
 * example4.cpp -- example 4, straight lines
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <common.h>
#include <debug.h>
#include <Line.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>

namespace csg {

/** 
 * \brief main function for example4
 */
int	main(int argc, char *argv[]) {
	int	c;
	int	phisteps = 4;
	double	radius = 0.1;
	while (EOF != (c = getopt(argc, argv, "r:ds:")))
		switch (c) {
		case 'd':
			debuglevel = LOG_DEBUG;
			break;
		case 'r':
			radius = atof(optarg);
			break;
		case 's':
			phisteps = atoi(optarg);
			break;
		}

	debug(LOG_DEBUG, DEBUG_LOG, 0, "3 lines of radius %f", radius);

	// build a polygon 
	Polyhedron	p1, p2, p3;

	// create three orthogonal lines 
	Interval	interval(-1, 2);
	Line	l1(point(), point(1,0,0));
	Line	l2(point(), point(0,1,0));
	Line	l3(point(), point(0,0,1));
	Build_Line	b1(l1, interval, phisteps, radius);
	Build_Line	b2(l2, interval, phisteps, radius);
	Build_Line	b3(l3, interval, phisteps, radius);
	p1.delegate(b1);
	p2.delegate(b2);
	p3.delegate(b3);
	fprintf(stderr, "lines created\n");

	// convert to Nef polyhedra
	Nef_polyhedron	n1(p1);
	Nef_polyhedron	n2(p2);
	n1 = n1 + n2;
	Nef_polyhedron	n3(p3);
	n1 = n1 + n3;

	// output union
	Polyhedron	P;
	n1.convert_to_polyhedron(P);
	std::cout << P;

	return EXIT_SUCCESS;
}

} // namespace csg

int	main(int argc, char *argv[]) {
	try {
		return csg::main(argc, argv);
	} catch (std::exception& x) {
		std::cerr << "terminated by std::exception: " << x.what()
			<< std::endl;
	} catch (...) {
		std::cerr << "terminated by unknown exception" << std::endl;
	}
}
