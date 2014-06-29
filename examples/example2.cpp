/*
 * example2.cpp -- example 2, arrow
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <common.h>
#include <Arrow.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <debug.h>

namespace csg {

/** 
 * \brief main function for example2
 */
int	main(int argc, char *argv[]) {
	int	c;
	int	steps = 16;
	double	radius = 0.1;
	while (EOF != (c = getopt(argc, argv, "r:dn:")))
		switch (c) {
		case 'd':
			debuglevel = LOG_DEBUG;
			break;
		case 'r':
			radius = atof(optarg);
			break;
		case 'n':
			steps = atoi(optarg);
			break;
		}

	debug(LOG_DEBUG, DEBUG_LOG, 0, "arrow of radius %f", radius);

	// build a polygon 
	Polyhedron	p1;

	// create spherical surfaces
	point	from(0, 0, 0);
	point	to(0, 0, 1);
	Build_Arrow	b1(from, to, radius, steps);
	p1.delegate(b1);
	fprintf(stderr, "arrow created\n");

	// convert to Nef polyhedra
	Nef_polyhedron	n1(p1);

	// output difference
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
