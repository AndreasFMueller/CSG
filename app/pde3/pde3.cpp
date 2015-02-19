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
#include <support.h>
#include <Parts.h>

namespace csg {

bool	support_enable = true;
bool	axes_enable = true;
bool	characteristics_enable = true;
bool	solution_enable = true;
bool	axessupport_enable = true;

double	thickness = 0.03;
int	steps = 20;
double	arrowdiameter = 0.04;
double	smallcurveradius = 0.04;
double	a = 1.5;
double	xa = 1.5;

double	f(double x) {
	if (x > xa) {
		return a / M_PI;
	}
	return a * x * x * (3 * xa - 2 * x) / (xa * xa * xa * M_PI);
}

int	main(int argc, char *argv[]) {
	int	c;
	while (EOF != (c = getopt(argc, argv, "dPXACS")))
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
		case 'A':
			axessupport_enable = false;
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
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add solution: %s",
			x.what());
	}

	// characteristics
	try {
		if (characteristics_enable) {
			unioner.add_polyhedron(build_characteristics());
		}
	} catch (std::exception& x) {
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add charactistics: %s",
			x.what());
	}

	// add the cut support
	try {
		if (support_enable) {
			unioner.add_polyhedron(
				build_cutsupport(2 * thickness));
		}
	} catch (std::exception& x) {
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add cut supports: %s",
			x.what());
	}

	// add the axes support
	try {
		if (axessupport_enable) {
			unioner.add_polyhedron(
				build_axessupport(thickness));
		}
	} catch (std::exception& x) {
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add axes supports: %s",
			x.what());
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
	debug(LOG_DEBUG, DEBUG_LOG, 0, "convert for output");
	Polyhedron	P;
	image.convert_to_polyhedron(P);
	std::cout << P;

	// output halves
	PartWriter	pw("nosolution");
	pw(PartWriter::BACK_PART, image, -0.01);
	pw(PartWriter::FRONT_PART, image, -0.01);
	pw(PartWriter::LEFT_PART, image, -0.01);
	pw(PartWriter::RIGHT_PART, image, -0.01);
	pw(PartWriter::TOP_PART, image, -0.01);
	pw(PartWriter::BOTTOM_PART, image, -0.01);

	debug(LOG_DEBUG, DEBUG_LOG, 0, "output complete");
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
