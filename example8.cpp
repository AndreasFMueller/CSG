/*
 * example8.cpp -- example 8, nonuniqueness of the solution of a partial
 *                            differential equation
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <common.h>
#include <debug.h>
#include <Box.h>
#include <SphericalSphere.h>
#include <Cartesian.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <math.h>
#include <Curve.h>

namespace csg {

double	a = 0.25;

class Solution : public PointFunction {
public:
	virtual point	p(double y0, double t) const {
		return point(y0 * sinh(t / y0), y0 * cosh(t / y0), a * y0 * y0);
	}
	virtual vector	v(double y0, double t) const {
		double	s = sinh(t / y0);
		double	c = cosh(t / y0);
		double	x = -2 * a * y0 * s;
		double	y = 2 * a * y0 * c;
		double	z = s * (s - (t / y0) * c) - c * (c - (t / y0) * s);
		return vector(x, y, z).normalized();
	}
};

class Basesolution : public Function {
public:
	virtual double	operator()(const double x, const double y) {
		return a * (y * y - x * x);
	}
};

class characteristic: public CurveFunction {
	double	y0;
public:
	characteristic(const double _y0, double deltat = 0)
		: CurveFunction(deltat), y0(_y0) { }
	virtual point	position(double t) const {
		point	p(y0 * sinh(t / y0), y0 * cosh(t / y0), a * y0 * y0);
		debug(LOG_DEBUG, DEBUG_LOG, 0, "t = %f, pos = (%f, %f, %f)",
			t, p.x(), p.y(), p.z());
		return p;
	}
	virtual vector	tangent(double t) const {
		vector	v(cosh(t / y0), sinh(t / y0), 0);
		debug(LOG_DEBUG, DEBUG_LOG, 0, "t = %f, tangent = (%f, %f, %f)",
			t, v.x(), v.y(), v.z());
		return v;
	}
	virtual vector	normal(double t) const {
		vector	n(sinh(t / y0) / y0, cosh(t / y0) / y0, 0);
		debug(LOG_DEBUG, DEBUG_LOG, 0, "t = %f, normal = (%f, %f, %f)",
			t, n.x(), n.y(), n.z());
		return n;
	}
};

/** 
 * \brief main function for example6
 */
int	main(int argc, char *argv[]) {
	int	c;
	while (EOF != (c = getopt(argc, argv, "d")))
		switch (c) {
		case 'd':
			if (debuglevel == LOG_DEBUG) {
				debuglevel++;
			} else {
				debuglevel = LOG_DEBUG;
			}
			break;
		}

	debug(LOG_DEBUG, DEBUG_LOG, 0, "sphere cut by plane");

	// build a polygon 
	Polyhedron	p1;

	// create a the solution surface
	int	steps = 30;
	CartesianDomain	domain(Interval(0, 2), Interval(-2, 2));
	Basesolution	basesolution;
	Build_CartesianFunction	b1(basesolution,
				domain, steps, 2 * steps, 0.02);
	p1.delegate(b1);
	Nef_polyhedron	n1(p1);

	characteristic	cs(1);
	Interval	interval(0, 2);
	Build_Curve	charcurve(cs, interval, steps, 8, 0.03);
	Polyhedron	p;
	p.delegate(charcurve);
	debug(LOG_DEBUG, DEBUG_LOG, 0, "convert curve to Nef_polyhedron");
	Nef_polyhedron	n(p);

	// output 
	Polyhedron	P;
	debug(LOG_DEBUG, DEBUG_LOG, 0, "build union");
	Nef_polyhedron	sum = n1 + n;
	debug(LOG_DEBUG, DEBUG_LOG, 0, "convert to polyhedron for output");
	sum.convert_to_polyhedron(P);
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
