/*
 * curves.cpp -- initial curve and such
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <curves.h>
#include <parameters.h>

namespace csg {
//////////////////////////////////////////////////////////////////////
// InitialCurve
//////////////////////////////////////////////////////////////////////

point	InitialCurve::position(double t) const {
	return point(0., t, a * t * t);
}

vector	InitialCurve::tangent(double t) const {
	return vector(0., 1., 2 * a * t);
}

vector	InitialCurve::normal(double t) const {
	return vector(0., 0., 2 * a);
}

Nef_polyhedron	build_initialcurve() {
	Polyhedron	p;
	Interval	interval(-2.1, 2.1);
	InitialCurve	ic;
	Build_Curve	initialcurve(ic, interval,
				steps, phisteps, largecurveradius);
	p.delegate(initialcurve);
	return Nef_polyhedron(p);
}

//////////////////////////////////////////////////////////////////////
// FCurve
//////////////////////////////////////////////////////////////////////

point	FCurve::position(double t) const {
	return point(t, 0., -a * t * t);
}

vector	FCurve::tangent(double t) const {
	return vector(1., 0., -2 * a * t);
}

vector	FCurve::normal(double t) const {
	return vector(0., 0., -2 * a);
}

Nef_polyhedron	build_fcurve() {
	Polyhedron	p;
	Interval	interval(0, 3);
	FCurve	fc;
	Build_Curve	fcurve(fc, interval,
				steps, phisteps, largecurveradius);
	p.delegate(fcurve);
	return Nef_polyhedron(p);
}

}
