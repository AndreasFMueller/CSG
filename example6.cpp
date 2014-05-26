/*
 * example6.cpp -- example 6, Box
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <common.h>
#include <Box.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>

namespace csg {

/** 
 * \brief main function for example6
 */
int	main(int argc, char *argv[]) {
	int	c;
	while (EOF != (c = getopt(argc, argv, "d")))
		switch (c) {
		case 'd':
			debug++;
			break;
		}

	if (debug) {
		fprintf(stderr, "%s:%d: box\n", __FILE__, __LINE__ );
	}

	// build a polygon 
	Polyhedron	p1;

	// create spherical surfaces
	point	from(-1, -2, -1.5);
	point	to(1, 2, 1.5);
	Build_Box	b1(from, to);
	p1.delegate(b1);
	fprintf(stderr, "Box created\n");

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
