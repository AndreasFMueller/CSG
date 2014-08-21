/*
 * support.cpp -- build a support sheet for the x-axis
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <support.h>
#include <parameters.h>
#include <debug.h>
#include <Cartesian.h>

namespace csg {

class point2 {
public:
	double x;
	double y;
	point2(double _x, double _y) : x(_x), y(_y) { }
};

class hyperbola {
	double	a, b, c2;
public:
	hyperbola(const point2& p1, const point2& p2, double c = 1) {
		double	sign = (c > 0) ? 1. : -1.;
		double	deltax = p2.x - p1.x;
		double 	deltay = p2.y - p1.y;
		c2 = c * c * ((deltax * deltay > 0) ? -1 : 1);
		double	k1 = p1.x * p1.y - c2;
		double	k2 = p2.x * p2.y - c2;
		double	deltak = k2 - k1;
		debug(LOG_DEBUG, DEBUG_LOG, 0,
			"deltax = %f, deltay = %f, deltak = %f",
			deltax, deltay, deltak);
		double	A = deltay / deltax;
		double	B = p1.y - p1.x * deltay/deltax - deltak/deltax;
		double	C = p1.x * deltak / deltax - k1;
		debug(LOG_DEBUG, DEBUG_LOG, 0, "A = %f, B = %f, C = %f",
			A, B, C);
		a = (-B + sign * sqrt(B * B - 4 * A * C)) / (2 * A);
		b = (deltak - a * deltay) / deltax;
		debug(LOG_DEBUG, DEBUG_LOG, 0, "a = %f, b = %f, c2 = %f",
			a, b, c2);
	}
	double	operator()(const double x) const {
		double	y = b + c2 / (x - a);
		debug(LOG_DEBUG, DEBUG_LOG, 0, "x = %f -> y = %f", x, y);
		return y;
	}
};

class axissupport : public PointFunction {
	hyperbola	top;
	double	bottom(double x) const {
		return exp(-x) * sin(-x * x / 2);
	}
public:
	axissupport() : top(point2(0,1), point2(1.8, 0.01)) { }
	virtual point	p(double x, double z) const {
		double	t = top(x);
		double	b = bottom(x);
		return point(x, 0, z * t + (1 - z) * b);
	}
	virtual vector	v(double x, double z) const {
		return vector::e2;
	}
};

Nef_polyhedron	build_support(double thickness) {
	CartesianDomain	domain(Interval(0, 1.8), Interval(0, 1));
	axissupport	support;
	Build_CartesianPointFunction	s(support, domain, 20, 2, thickness);
	Polyhedron	p;
	p.delegate(s);
	return Nef_polyhedron(p);
}

} // namespace csg
