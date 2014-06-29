/*
 * example5.cpp -- example 5, solution of partial differential equation by
 *                            characteristics
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <common.h>
#include <debug.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <solution.h>
#include <characteristics.h>
#include <axes.h>
#include <initialcurve.h>
#include <Parts.h>
#include <Box.h>

namespace csg {

int	phisteps = 4;
int	curvesteps = 10;
double	radius = 0.1;
std::string	prefix("characteristics");

/** 
 * \brief main function for example5
 */
int	main(int argc, char *argv[]) {
	int	c;
	bool	initialcurve = true;
	bool	solutionsurface = true;
	bool	characteristics = true;
	while (EOF != (c = getopt(argc, argv, "r:ds:p:ICSc:")))
		switch (c) {
		case 'c':
			curvesteps = atoi(optarg);
			break;
		case 'd':
			debuglevel = LOG_DEBUG;
			break;
		case 'I':
			initialcurve = false;
			break;
		case 'r':
			radius = atof(optarg);
			break;
		case 'S':
			solutionsurface = false;
			break;
		case 'C':
			characteristics = false;
			break;
		case 's':
			phisteps = atoi(optarg);
			break;
		case 'p':
			prefix = std::string(optarg);
			break;
		}

	debug(LOG_DEBUG, DEBUG_LOG, 0, "3 lines of radius %f", radius);

	// here is Nef_nary_union which we use to build up the image
	Nef_nary_union	unioner;

	// adding the initial curve
	if (initialcurve) {
		try {
			unioner.add_polyhedron(build_initialcurve(curvesteps, phisteps));
		} catch (std::exception& x) {
			fprintf(stderr, "exception while adding initial curve: %s\n", x.what());
		} catch (...) {
			fprintf(stderr, "exception while adding initial curve\n");
		}
	} else {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "initial curve suppressed");
	}

	// adding the characteristics
	if (characteristics) {
		try {
			// create the characteristic curves
			debug(LOG_DEBUG, DEBUG_LOG, 0,
				"adding characteristics to image");
			unioner.add_polyhedron(
				build_characteristics(phisteps, curvesteps));
		} catch (std::exception& x) {
			fprintf(stderr, "exception while adding characteristics: %s\n", x.what());
		} catch (...) {
			fprintf(stderr, "exception while adding characteristics\n");
		}
	} else {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "characterstics suppressed");
	}

	// adding the solution surface
	if (solutionsurface) {
		try {
			debug(LOG_DEBUG, DEBUG_LOG, 0,
				"adding solution surface");
			unioner.add_polyhedron(build_solution());
		} catch (std::exception& x) {
			fprintf(stderr, "exception while adding solution "
				"surface: %s\n", x.what());
		} catch (...) {
			fprintf(stderr, "exception while adding solution "
				"surface\n");
		}
	} else {
		debug(LOG_DEBUG, DEBUG_LOG, 0,
			"solution surface suppressed");
	}

	// restrict what we have so far to a box
	Build_Box	box(point(-0.1, 0, -1), point(1.8, M_PI, 1.1));
	Polyhedron	boxp;
	boxp.delegate(box);
	Nef_polyhedron	image = Nef_polyhedron(boxp) * unioner.get_union();

	// now add the various components, starting with the X-axis
	image = image + build_axes();

	// output union
	Polyhedron	P;
	image.convert_to_polyhedron(P);
	std::cout << P;

	// now we have to cut along the y-z-plane, because otherwise it would
	// hardly be printable
	PartWriter	pw(prefix);
	pw(PartWriter::LEFT_PART, image);
	pw(PartWriter::RIGHT_PART, image);

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
