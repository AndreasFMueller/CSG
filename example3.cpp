/*
 * example3.cpp -- example 3, helix
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <common.h>
#include <Curve.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <debug.h>

namespace csg {

class Helix : public CurveFunction {
	double	_radius;
	double	_a;
public:
	Helix(double radius, double a)
		: CurveFunction(), _radius(radius), _a(a) { }

	virtual point	position(double t) const {
		return point(_radius * cos(t), _radius * sin(t), _a * t);
	}

	virtual vector	tangent(double t) const {
		return vector(-_radius * sin(t), _radius * cos(t), _a);
	}

	virtual vector	normal(double t) const {
		return vector(-cos(t), -sin(t), 0);
	}
};

/** 
 * \brief main function for example3
 */
int	main(int argc, char *argv[]) {
	int	c;
	int	steps = 100;
	int	phisteps = 4;
	double	radius = 0.1;
	while (EOF != (c = getopt(argc, argv, "r:dn:s:")))
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
		case 's':
			phisteps = atoi(optarg);
			break;
		}

	debug(LOG_DEBUG, DEBUG_LOG, 0, "arrow of radius %f", radius);

	// build a polygon 
	Polyhedron	p1;

	// create a curve
	Interval	threeturns(0, 6 * M_PI);
	Helix	helix(1, 2 / threeturns.length());
	Build_Curve	b1(helix, threeturns, steps, phisteps, radius);
	p1.delegate(b1);
	fprintf(stderr, "curve created\n");

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
