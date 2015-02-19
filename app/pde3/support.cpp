/*
 * support.cpp -- create support surfaces for
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <support.h>
#include <parameters.h>
#include <Cartesian.h>
#include <hyperbola.h>
#include <debug.h>

namespace csg {

double	cutsupport::top(double x) const {
	//return _sign * (f(x) + 0.001) * M_PI;
	return _sign * (h(x) + 0.005);
}

point	cutsupport::p(double x, double z) const {
	double  t = top(x);
	return point(x, 0 * _y, z * t);
}

vector	cutsupport::v(double /* x */, double /* z */) const {
	return vector::e2;
}	

Nef_polyhedron	build_cutsupport(double thickness) {
	CartesianDomain	domain(Interval(0, 2), Interval(0, 1));

	cutsupport	support1(thickness / 2);
	Build_CartesianPointFunction	s1(support1, domain, 20, 2, thickness);
	Polyhedron	p1;
	p1.delegate(s1);

	cutsupport	support2(-thickness / 2, -1);
	Build_CartesianPointFunction	s2(support2, domain, 20, 2, thickness);
	Polyhedron	p2;
	p2.delegate(s2);

	return Nef_polyhedron(p1) + Nef_polyhedron(p2);
}

class axessupport1 : public PointFunction {
	hyperbola	bottom;
public:
	axessupport1() : bottom(point2(0, -a), point2(2, 0), -1) { }
	virtual	point	p(double y, double z) const {
		double	v1 = bottom(y) - 0.002;
		double	v2 = -f(y) * M_PI / 2;
		double	v = std::min(v1, v2);
		// debug(LOG_DEBUG, DEBUG_LOG, 0, "y = %f -> %f", y, v);
		return point(0, y, z * v);
	}
	virtual vector	v(double /* y */ , double /* z */) const {
		return vector::e1;
	}
};

class axessupport2 : public PointFunction {
	hyperbola	top;
public:
	axessupport2() : top(point2(-2, 0), point2(0, a), 1) { }
	virtual	point	p(double y, double z) const {
		double	v1 = top(y) + 0.002;
		double	v2 = f(-y) * M_PI / 2;
		double	v = std::max(v1, v2);
		// debug(LOG_DEBUG, DEBUG_LOG, 0, "y = %f -> %f", y, v);
		return point(0, y, z * v);
	}
	virtual vector	v(double /* y */, double /* z */) const {
		return vector::e1;
	}
};

Nef_polyhedron	build_axessupport(double thickness) {
	CartesianDomain	domain1(Interval(0, 2), Interval(0, 1));
	axessupport1	positive;
	Build_CartesianPointFunction	s1(positive, domain1, 20, 2, thickness);
	Polyhedron	p1;
	p1.delegate(s1);

	CartesianDomain	domain2(Interval(-2, 0), Interval(0, 1));
	axessupport2	negative;
	Build_CartesianPointFunction	s2(negative, domain2, 20, 2, thickness);
	Polyhedron	p2;
	p2.delegate(s2);

	return Nef_polyhedron(p1) + Nef_polyhedron(p2);
}

} // namespace csg
