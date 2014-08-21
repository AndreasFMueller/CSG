/*
 * pde1.cpp -- application pde1
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <parameters.h>
#include <common.h>
#include <debug.h>
#include <Cartesian.h>
#include <math.h>
#include <debug.h>

namespace csg {

/**
 * \brief Support along the x-axis
 */
class SupportSheet : public PointFunction {
public:
	virtual point	p(double x, double z) const {
		return point(x, 0, -z - a * x * x);
	}
	virtual vector	v(double x, double z) const {
		return vector::e2;
	}
};

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

/**
 * \brief Support of the initial curve
 */
class CharSupport : public PointFunction {
	hyperbola	top;
	hyperbola	bottom;
#if 0
	double	b;
	double	bottom(double y) const {
		double	z = b - (2 + b) * b / (fabs(y) + b);
		//debug(LOG_DEBUG, DEBUG_LOG, 0, "bottom(%f) = %f", y, z);
		return z - 0.01;
	}
	double	top(double y) const {
		double	z = -b + 2 * (1 + b) * (2 + b) / (fabs(y) + 2 * (1 + b));
		//debug(LOG_DEBUG, DEBUG_LOG, 0, "top(%f) = %f", y, z);
		return z + 0.01;
	}
#endif
public:
	CharSupport(double _b = 1)
		: top(point2(0, 2.0), point2(2, 1), 0.5),
		  bottom(point2(0, -2), point2(2, 0), -0.8) {
	}
	virtual point	p(double y, double z) const {
		double	t = top(fabs(y));
		double	b = bottom(fabs(y));
		return point(0, y, z * t + (1 - z) * b);
	}
	virtual vector	v(double y, double z) const {
		return vector::e1;
	}
};

Nef_polyhedron	build_support(double thickness) {
	if (!yzslicing) {
		CartesianDomain	domain(Interval(-2, 2), Interval(0, 1));
		CharSupport	support;
		Build_CartesianPointFunction	s(support,
				domain, 4 * steps, 2, thickness);
		Polyhedron	p;
		p.delegate(s);
		return	Nef_polyhedron(p);
	} else {
		CartesianDomain	domain(Interval(0, 4), Interval(0, 2));
		SupportSheet	support;
		Build_CartesianPointFunction	s(support,
				domain, steps, steps, thickness);
		Polyhedron	p;
		p.delegate(s);
		return	Nef_polyhedron(p);
	}
}

} // namespace csg
