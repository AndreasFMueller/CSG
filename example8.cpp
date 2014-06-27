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
#include <CGAL/Nef_nary_union_3.h>

namespace csg {

typedef CGAL::Nef_nary_union_3<Nef_polyhedron>  Nef_nary_union;

const double	a = 0.25;
const double	speed = 1;
int	steps = 30;

double	h0(double x) {
	return exp(-log(2) * 0.5 * 0.5 / (x * x));
}

double	h(double x) {
	if (x <= 0) {
		return 0;
	}
	if (x <= 0.5) {
		return h0(x);
	}
	if (x < 1) {
		return 1 - h0(1 - x);
	}
	return 1;
}

double	f(double x, double mu) {
	return h(x / speed) * mu * x - (1 - h(x / speed)) * a * x * x;
}

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

class BaseSolution : public Function {
public:
	virtual double	operator()(const double x, const double y) {
		return a * (y * y - x * x);
	}
};

class AlternativeSolution : public PointFunction {
	double	mu;
public:
	AlternativeSolution(double _mu) : mu(_mu) { }
	virtual point	p(double xi, double eta) const {
		double	x = 2 * (eta * eta + xi * xi);
		double	y = 2 * (eta * eta - xi * xi);
		return point(x, y, f(sqrt(x * x - y * y), mu));
	}
	virtual vector	v(double xi, double eta) const {
		return vector::e3;
	}
};

class CharacteristicY : public CurveFunction {
	double	y0;
public:
	CharacteristicY(const double _y0, double deltat = 0)
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

class CharacteristicX : public CurveFunction {
	double	x0;
	double	z;
public:
	CharacteristicX(const double _x0, double _z, double deltat = 0)
		: CurveFunction(deltat), x0(_x0), z(_z) { }
	virtual point	position(double t) const {
		point	p(x0 * cosh(t / x0), x0 * sinh(t / x0), z);
		return p;
	}
	virtual vector	tangent(double t) const {
		vector	v(sinh(t / x0), cosh(t / x0), 0);
		return v;
	}
	virtual	vector	normal(double t) const {
		vector	n(cosh(t / x0) / x0, sinh(t / x0) / x0, 0);
		return n;
	}
};

static Nef_polyhedron	build_alternative() {
	debug(LOG_DEBUG, DEBUG_LOG, 0, "build alternative solution surfaces");
	Nef_nary_union	unioner;
	CartesianDomain	xietadomain(Interval(0, 1), Interval(0, 1));
	for (double m = 0.5; m > -0.5; m -= 0.2) {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "m = %f", m);
		AlternativeSolution	altsolution(0.5);
		Build_CartesianPointFunction	a(altsolution,
			xietadomain, steps, steps, 0.02);
		Polyhedron	p;
		p.delegate(a);
		Nef_polyhedron	n(p);
		unioner.add_polyhedron(n);
	}
	debug(LOG_DEBUG, DEBUG_LOG, 0, "get union");
	return unioner.get_union();
}

static void	add_characteristic(Nef_nary_union& unioner, double y0) {
	Interval	interval(0, 2);
	debug(LOG_DEBUG, DEBUG_LOG, 0, "add characteristic at y0 = %f", y0);
	CharacteristicY	cs(y0);
	Build_Curve	charcurve(cs, interval, steps, 8, 0.03);
	Polyhedron	p;
	p.delegate(charcurve);
	Nef_polyhedron	n(p);
	unioner.add_polyhedron(n);
	debug(LOG_DEBUG, DEBUG_LOG, 0, "curve added");
}

static Nef_polyhedron	build_characteristics() {
	Nef_nary_union	unioner;
	Interval	interval(0, 2);
	for (double y0 = 0.5; y0 <= 2.01; y0 += 0.4) {
		add_characteristic(unioner, y0);
		add_characteristic(unioner, -y0);
	}
	return unioner.get_union();
}

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
	CartesianDomain	domain(Interval(0, 2), Interval(-2, 2));
	BaseSolution	basesolution;
	Build_CartesianFunction	b1(basesolution,
					domain, steps, 2 * steps, 0.02);
	p1.delegate(b1);
	Nef_polyhedron	n1(p1);

	// create an alternative solution surface
	Nef_polyhedron	n2 = build_alternative();

	// add additional characteristics
	Nef_polyhedron	n3 = build_characteristics();

	// output 
	Polyhedron	P;
	debug(LOG_DEBUG, DEBUG_LOG, 0, "build union");
	Nef_polyhedron	sum = n1 + n2 + n3;
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
