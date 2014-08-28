/*
 * pde3.cpp -- 1st order PDE without solutions
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdlib.h>
#include <parameters.h>
#include <debug.h>
#include <common.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <solution.h>
#include <axes.h>
#include <characteristics.h>
#include <math.h>

namespace csg {

bool	support_enable = true;
bool	axes_enable = true;
bool	initial_enable = true;
bool	characteristics_enable = true;
bool	solution_enable = true;

double	thickness = 0.03;
int	steps = 20;
double	arrowdiameter = 0.04;
double	smallcurveradius = 0.04;
double	a = 2.;
double	xa = 1.5;

double	f(double x) {
	if (x > xa) {
		return a / M_PI;
	}
	return a * x * x * (3 * xa - 2 * x) / (xa * xa * xa * M_PI);
}

int	main(int argc, char *argv[]) {
	int	c;
	while (EOF != (c = getopt(argc, argv, "dPXICS")))
		switch (c) {
		case 'd':
			if (debuglevel == LOG_DEBUG) {
				debuglevel++;
			} else {
				debuglevel = LOG_DEBUG;
			}
			break;
		case 'P':
			support_enable = false;
			break;
		case 'X':
			axes_enable = false;
			break;
		case 'I':
			initial_enable = false;
			break;
		case 'C':
			characteristics_enable = false;
			break;
		case 'S':
			solution_enable = false;
			break;
		}

	Nef_nary_union	unioner;

	// solution
	try {
		if (solution_enable) {
			unioner.add_polyhedron(build_solution(thickness));
		}
	} catch (std::exception& x) {
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add solution: %s", x.what());
	}

	// characteristics
	try {
		if (characteristics_enable) {
			unioner.add_polyhedron(build_characteristics());
		}
	} catch (std::exception& x) {
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add charactistics: %s", x.what());
	}

	// axes
	try {
		if (axes_enable) {
			unioner.add_polyhedron(build_axes());
		}
	} catch (std::exception& x) {
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add axes: %s", x.what());
	}

	// extract
	debug(LOG_DEBUG, DEBUG_LOG, 0, "extracting image");
	Nef_polyhedron	image = unioner.get_union();

	// output
	debug(LOG_DEBUG, DEBUG_LOG, 0, "covert for output");
	Polyhedron	P;
	image.convert_to_polyhedron(P);
	std::cout << P;

	return EXIT_SUCCESS;
}

} // namespace csg

int	main(int argc, char *argv[]) {
	try {
		return csg::main(argc, argv);
	} catch (std::exception& x) {
		std::cerr << "terminated by exception: " << x.what() << std::endl;
	} catch (...) {
		std::cerr << "terminated by unknown exception" << std::endl;
	}
	return EXIT_FAILURE;
}
