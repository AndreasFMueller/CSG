/*
 * pde1.cpp -- application pde1
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <parameters.h>
#include <solution.h>
#include <characteristics.h>
#include <curves.h>
#include <axes.h>
#include <Parts.h>
#include <getopt.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <common.h>
#include <support.h>
#include <debug.h>
#include <Box.h>
#include <Cartesian.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>

namespace csg {

std::string	prefix("hypchar");
double	a = 0.25;
double	speed = 4;
int	steps = 30;
double	charstep = 0.25;
int	phisteps = 12;

// thickness of solution and support sheets
double	sheetthickness = 0.05;
double	smallcurveradius = 0.04;
double	arrowdiameter = 0.04;
double	largecurveradius = 0.060;

#if 0
class SupportSheet : public PointFunction {
public:
	virtual point	p(double x, double z) const {
		return point(x, 0, -z - a * x * x);
	}
	virtual vector	v(double x, double z) const {
		return vector::e2;
	}
};

class CharSupport : public PointFunction {
public:
	virtual point	p(double y, double z) const {
		return point(0, y, (a * y * y + 0.001) * z);
	}
	virtual vector	v(double y, double z) const {
		return vector::e1;
	}
};

static Nef_polyhedron	build_support() {
	if (!yzslicing) {
		CartesianDomain	domain(Interval(-2, 2), Interval(0, 1));
		CharSupport	support;
		Build_CartesianPointFunction	s(support,
				domain, steps, steps, sheetthickness);
		Polyhedron	p;
		p.delegate(s);
		return	Nef_polyhedron(p);
	} else {
		CartesianDomain	domain(Interval(0, 4), Interval(0, 2));
		SupportSheet	support;
		Build_CartesianPointFunction	s(support,
				domain, steps, steps, sheetthickness);
		Polyhedron	p;
		p.delegate(s);
		return	Nef_polyhedron(p);
	}
}
#endif

bool	show_solution = true;
bool	show_characteristics = true;
bool	show_alternatives = true;
bool	show_initial = true;
bool	show_axes = true;
bool	show_negative = true;
bool	show_support = false;
bool	show_fcurve = true;

/** 
 * \brief main function for example6
 */
int	main(int argc, char *argv[]) {

	int	c;
	while (EOF != (c = getopt(argc, argv, "dSACIXNPFx")))
		switch (c) {
		case 'd':
			if (debuglevel == LOG_DEBUG) {
				debuglevel++;
			} else {
				debuglevel = LOG_DEBUG;
			}
			break;
		case 'S':
			show_solution = !show_solution;
			break;
		case 'A':
			show_alternatives = !show_alternatives;
			break;
		case 'C':
			show_characteristics = !show_characteristics;
			break;
		case 'I':
			show_initial = !show_initial;
			break;
		case 'X':
			show_axes = !show_axes;
			break;
		case 'N':
			show_negative = !show_negative;
			break;
		case 'F':
			show_fcurve = !show_fcurve;
			break;
		case 'P':
			show_support = !show_support;
			break;
		case 'p':
			prefix = std::string(optarg);
			break;
		case 'x':
			yzslicing = false;
			break;
		}

	debug(LOG_DEBUG, DEBUG_LOG, 0, "nonuniqueness of solution of a "
		"partial differential equation");

	// start building up the union of things to be restricted to a box
	Nef_nary_union	unioner;

	// create a the solution surface
	try {
		if (show_solution) {
			debug(LOG_DEBUG, DEBUG_LOG, 0, "adding solution surface");
			unioner.add_polyhedron(build_solution(sheetthickness));
			debug(LOG_DEBUG, DEBUG_LOG, 0, "solution surface added");
		}
	} catch (std::exception& x) {
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add solution: %s",
			x.what());
	}

	// create an alternative solution surface
	try {
		if (show_alternatives) {
			debug(LOG_DEBUG, DEBUG_LOG, 0, "adding alternatives");
			unioner.add_polyhedron(build_alternative(sheetthickness));
			debug(LOG_DEBUG, DEBUG_LOG, 0, "alternatives added");
		}
	} catch (std::exception& x) {
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add alternatives: %s",
			x.what());
	}

	// add additional characteristics
	try {
		if (show_characteristics) {
			debug(LOG_DEBUG, DEBUG_LOG, 0, "adding characteristics");
			unioner.add_polyhedron(build_characteristics());
			debug(LOG_DEBUG, DEBUG_LOG, 0, "characteristics added");
		}
	} catch (std::exception& x) {
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add characteristics: %s",
			x.what());
	}

	// add negative characteristics
	try {
		if (show_negative) {
			debug(LOG_DEBUG, DEBUG_LOG, 0, "adding neg characteristics");
			unioner.add_polyhedron(build_xcharacteristics());
			debug(LOG_DEBUG, DEBUG_LOG, 0, "neg characteristics added");
		}
	} catch (std::exception& x) {
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add negative characteristics: %s",
			x.what());
	}

	// add the FCurve
	try {
		if (show_fcurve) {
			debug(LOG_DEBUG, DEBUG_LOG, 0, "adding fcurve");
			unioner.add_polyhedron(build_fcurve());
			debug(LOG_DEBUG, DEBUG_LOG, 0, "fcurve added");
		}
	} catch (std::exception& x) {
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add fcurve: %s",
			x.what());
	}

	// add the support sheet
	try {
		if (show_support) {
			debug(LOG_DEBUG, DEBUG_LOG, 0, "adding support");
			unioner.add_polyhedron(build_support(sheetthickness));
			debug(LOG_DEBUG, DEBUG_LOG, 0, "support added");
		}
	} catch (std::exception& x) {
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add support: %s",
			x.what());
	}

	// add the initial curve
	if (show_initial) {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "adding initial curve");
		unioner.add_polyhedron(build_initialcurve());
		debug(LOG_DEBUG, DEBUG_LOG, 0, "initial curve added");
	}

	// extract union
	debug(LOG_DEBUG, DEBUG_LOG, 0, "extract the image component union");
	Nef_polyhedron	image = unioner.get_union();
	debug(LOG_DEBUG, DEBUG_LOG, 0, "base image constructed");

	// restrict everything to a box
	debug(LOG_DEBUG, DEBUG_LOG, 0, "restrict to a box");
	Build_Box	box(point(-0.1, -2, -2), point(4, 2, 2));
        Polyhedron      boxp;
        boxp.delegate(box);
	image = image * boxp;
	debug(LOG_DEBUG, DEBUG_LOG, 0, "restriction complete");

	// add axes
	if (show_axes) {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "adding axes");
		image = image + build_axes();
		debug(LOG_DEBUG, DEBUG_LOG, 0, "axes added");
	}

	// output 
	Polyhedron	P;
	debug(LOG_DEBUG, DEBUG_LOG, 0, "convert to polyhedron for output");
	image.convert_to_polyhedron(P);
	std::cout << P;

	// write parts
	PartWriter	pw(prefix);
	pw(PartWriter::LEFT_PART, image);
	pw(PartWriter::RIGHT_PART, image);
	pw(PartWriter::FRONT_PART, image);
	pw(PartWriter::BACK_PART, image);

	// that's it
	debug(LOG_DEBUG, DEBUG_LOG, 0, "output complete");
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
