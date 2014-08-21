/*
 * solution.cpp -- solution functions for pde1
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <solution.h>
#include <debug.h>
#include <Cartesian.h>
#include <math.h>
#include <parameters.h>
#include <CGAL/Nef_nary_union_3.h>

namespace csg {

typedef CGAL::Nef_nary_union_3<Nef_polyhedron>  Nef_nary_union;

bool	yzslicing = true;

static double	h0(double x) {
	return exp(-log(2) * 0.5 * 0.5 / (x * x));
}

static double	h(double x) {
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

static const double speed = 4;

static double	f(double x, double mu) {
	return h(x / speed) * mu * x - (1 - h(x / speed)) * a * x * x;
}

//////////////////////////////////////////////////////////////////////
// Solution implementation
//////////////////////////////////////////////////////////////////////

point	Solution::p(double y0, double t) const {
	return point(y0 * sinh(t / y0), y0 * cosh(t / y0), a * y0 * y0);
}

vector	Solution::v(double y0, double t) const {
	double	s = sinh(t / y0);
	double	c = cosh(t / y0);
	double	x = -2 * a * y0 * s;
	double	y = 2 * a * y0 * c;
	double	z = s * (s - (t / y0) * c) - c * (c - (t / y0) * s);
	if (yzslicing) {
		return vector(0, y, z).normalized();
	} else {
		return vector(x, 0, z).normalized();
	}
}

Nef_polyhedron	build_solution(double thickness) {
	debug(LOG_DEBUG, DEBUG_LOG, 0, "building solution surface");
	Polyhedron	p;
	CartesianDomain	domain(Interval(0, 4), Interval(-2, 2));
	BaseSolution	basesolution;
	Build_CartesianPointFunction	b(basesolution, domain,
		2 * steps, 2 * steps, thickness);
	p.delegate(b);
	return Nef_polyhedron(p);
}

//////////////////////////////////////////////////////////////////////
// BaseSolution implementation
//////////////////////////////////////////////////////////////////////

point	BaseSolution::p(double x, double y) const {
	return point(x, y, a * (y * y - x * x));
}

vector	BaseSolution::v(double x, double y) const {
	if (yzslicing) {
		double	derivative = 2 * a * y;
		return vector(0., -derivative, 1.).normalized();
	} else {
		double	derivative = -2 * a * x;
		return vector(-derivative, 0., 1.).normalized();
	}
}

//////////////////////////////////////////////////////////////////////
// AlternativeSolution implementation
//////////////////////////////////////////////////////////////////////

double	AlternativeSolution::F(double x, double y) const {
	if (fabs(x) <= fabs(y)) {
		return 0;
	}
	return f(sqrt(x * x - y * y), mu);
}

double	AlternativeSolution::x(double xi, double eta) const {
	return 4 * (pow(eta, gamma) + pow(xi, gamma));
}

double	AlternativeSolution::y(double xi, double eta) const {
	return 4 * (pow(eta, gamma) - pow(xi, gamma));
}

AlternativeSolution::AlternativeSolution(double _mu) : mu(_mu) { }

point	AlternativeSolution::p(double xi, double eta) const {
	double	xx = x(xi, eta);
	double	yy = y(xi, eta);
	return point(xx, yy, F(xx, yy));
}

vector	AlternativeSolution::v(double xi, double eta) const {
	double	xx = x(xi, eta);
	double	yy = y(xi, eta);
	if (yzslicing) {
		double	dy = 0.0001;
		double	derivative = (F(xx, yy + dy) - F(xx, yy)) / dy;
		vector	n =  vector(0., -derivative, 1.).normalized();
		return n;
	} else {
		double	dx = 0.0001;
		double	derivative = (F(xx + dx, yy) - F(xx, yy)) / dx;
		vector	n =  vector(-derivative, 0., 1.).normalized();
		return n;
	}
}

double	AlternativeSolution::gamma = 1.5;

Nef_polyhedron	build_alternative(double thickness) {
	debug(LOG_DEBUG, DEBUG_LOG, 0, "build alternative solution surfaces");
	Nef_nary_union	unioner;
	CartesianDomain	xietadomain(Interval(0, 1), Interval(0, 1));
	for (double m = 0.5; m > -0.6; m -= 0.25) {
	//for (double m = 0.25; m > 0.2; m -= 0.25) {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "m = %f", m);
		AlternativeSolution	altsolution(m);
		Build_CartesianPointFunction	a(altsolution,
			xietadomain, 2 * steps, 2 * steps, thickness);
		Polyhedron	p;
		p.delegate(a);
		Nef_polyhedron	n(p);
		unioner.add_polyhedron(n);
	}
	debug(LOG_DEBUG, DEBUG_LOG, 0, "get union");
	return unioner.get_union();
}

} // namespace csg
