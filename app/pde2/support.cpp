/*
 * support.cpp -- build a support sheet for the x-axis
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <support.h>
#include <parameters.h>
#include <debug.h>
#include <Cartesian.h>
#include <hyperbola.h>

namespace csg {

class axissupport : public PointFunction {
	hyperbola	top;
	double	bottom(double x) const {
		return exp(-x) * sin(-x * x / 2) - thickness / 4;
	}
public:
	axissupport() : top(point2(0,1), point2(1.8, 0.01)) { }
	virtual point	p(double x, double z) const {
		double	t = top(x);
		double	b = bottom(x);
		return point(x, 0, z * t + (1 - z) * b);
	}
	virtual vector	v(double /* x */, double /* z */) const {
		return vector::e2;
	}
};

class initialsupport : public PointFunction {
	hyperbola	top;
	double	bottom(double y) const {
		return sin(y) - thickness;
	}
public:
	initialsupport() : top(point2(0, 1), point2(0.5, sin(0.5))) { }
	virtual point	p(double y, double z) const {
		double	t = top(y);
		double	b = bottom(y);
		return point(0, y, z * t + (1 - z) * b);
	}
	virtual vector	v(double /* y */, double /* z */) const {
		return vector::e1;
	}
};

Nef_polyhedron	build_support(double thickness) {
	debug(LOG_DEBUG, DEBUG_LOG, 0, "building supports");

	axissupport	support1;
	CartesianDomain	domain1(Interval(0, 1.8), Interval(0, 1));
	Build_CartesianPointFunction	s1(support1, domain1, 20, 2, thickness);
	Polyhedron	p1;
	p1.delegate(s1);
	debug(LOG_DEBUG, DEBUG_LOG, 0, "x-axis support built");

	initialsupport	support2;
	CartesianDomain	domain2(Interval(0, 0.5), Interval(0, 1));
	Build_CartesianPointFunction	s2(support2, domain2, 20, 2, thickness);
	Polyhedron	p2;
	p2.delegate(s2);
	debug(LOG_DEBUG, DEBUG_LOG, 0, "z-axis support built");
	
	return Nef_polyhedron(p1) + Nef_polyhedron(p2);
}

} // namespace csg
