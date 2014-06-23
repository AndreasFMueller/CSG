/*
 * example6.cpp -- example 7, Sphere cut by a plane
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <common.h>
#include <Box.h>
#include <SphericalSphere.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>

namespace csg {

typedef Nef_polyhedron::Plane_3	Plane_3;

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

	// create a sphere
	double	radius = 4;
	int	steps = 30;
	Build_SphericalSphere	b1(radius, steps);
	p1.delegate(b1);

	// convert to Nef polyhedra
	Nef_polyhedron	n1(p1);

	// create a half space
	if (debug) {
		fprintf(stderr, "%s:%d: construct a plane\n",
			__FILE__, __LINE__);
	}
	Plane_3	plane(1,2,3,-1);

	if (debug) {
		fprintf(stderr, "%s:%d: construct a half space\n",
			__FILE__, __LINE__);
	}
	Nef_polyhedron	n2(plane, Nef_polyhedron::INCLUDED);

	if (debug) {
		fprintf(stderr, "%s:%d: intersect with half space\n",
			__FILE__, __LINE__);
	}
	Nef_polyhedron	n3 = n1 * n2; // intersection

	// output difference
	Polyhedron	P;
	n3.convert_to_polyhedron(P);
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
