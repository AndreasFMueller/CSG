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
#include <hyperbola.h>

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

/**
 * \brief Support of the initial curve
 */
class CharSupport : public PointFunction {
	hyperbola	top;
	hyperbola	bottom;
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
