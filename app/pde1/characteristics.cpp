/*
 * characteristics.cpp -- classes for characteristics
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <characteristics.h>
#include <debug.h>
#include <Cartesian.h>
#include <Curve.h>
#include <math.h>
#include <parameters.h>

namespace csg {

//////////////////////////////////////////////////////////////////////
// CharacteristicY
//////////////////////////////////////////////////////////////////////

point	CharacteristicY::position(double t) const {
	point	p(y0 * sinh(t / y0), y0 * cosh(t / y0), a * y0 * y0);
	return p;
}

vector	CharacteristicY::tangent(double t) const {
	vector	v(cosh(t / y0), sinh(t / y0), 0);
	return v;
}

vector	CharacteristicY::normal(double t) const {
	vector	n(sinh(t / y0) / y0, cosh(t / y0) / y0, 0);
	return n;
}

//////////////////////////////////////////////////////////////////////
// CharacteristicX
//////////////////////////////////////////////////////////////////////

point	CharacteristicX::position(double t) const {
	point	p(x0 * cosh(t / x0), x0 * sinh(t / x0), z);
	return p;
}

vector	CharacteristicX::tangent(double t) const {
	vector	v(sinh(t / x0), cosh(t / x0), 0);
	return v;
}

vector	CharacteristicX::normal(double t) const {
	vector	n(cosh(t / x0) / x0, sinh(t / x0) / x0, 0);
	return n;
}

//////////////////////////////////////////////////////////////////////
// Asymptote
//////////////////////////////////////////////////////////////////////

point	Asymptote::position(double t) const {
	return point(t, m * t, 0.);
}

vector	Asymptote::tangent(double /* t */) const {
	return vector(1., m, 0.);
}

vector	Asymptote::normal(double /* t */) const {
	return vector::e2;
}

//////////////////////////////////////////////////////////////////////
// build_xcharacteristics implementation
//////////////////////////////////////////////////////////////////////

static void	add_characteristic(Nef_nary_union& unioner, double y0) {
	debug(LOG_DEBUG, DEBUG_LOG, 0, "add characteristic at y0 = %f", y0);
	double	tmax = fabs(y0) * acosh(2 / fabs(y0));
	debug(LOG_DEBUG, DEBUG_LOG, 0, "t interval extends to %f", tmax);
	Interval	interval(0, tmax + 0.2);
	CharacteristicY	cs(y0);
	Build_Curve	charcurve(cs, interval, steps, 8,
				smallcurveradius);
	Polyhedron	p;
	p.delegate(charcurve);
	Nef_polyhedron	n(p);
	unioner.add_polyhedron(n);
	debug(LOG_DEBUG, DEBUG_LOG, 0, "curve added");
}

static void	add_asymptote(Nef_nary_union& unioner, double m) {
	debug(LOG_DEBUG, DEBUG_LOG, 0, "adding asymptote %f", m);
	Interval	interval(0., 2.1);
	Asymptote	asymptote(m);
	Build_Curve	charcurve(asymptote, interval, steps, phisteps,
				smallcurveradius);
	Polyhedron	p;
	p.delegate(charcurve);
	Nef_polyhedron	n(p);
	unioner.add_polyhedron(n);
	debug(LOG_DEBUG, DEBUG_LOG, 0, "asymptote %f added", m);
}

Nef_polyhedron	build_characteristics() {
	Nef_nary_union	unioner;
	debug(LOG_DEBUG, DEBUG_LOG, 0, "add 1st quadrant characteristics");
	for (double y0 = charstep * floor(2 / charstep); y0 > 0.1;
			y0 -= charstep) {
		add_characteristic(unioner, y0);
	}
	debug(LOG_DEBUG, DEBUG_LOG, 0, "add asymptotes");
	add_asymptote(unioner, 1);
	add_asymptote(unioner, -1);
	debug(LOG_DEBUG, DEBUG_LOG, 0, "add 4th quadrant characteristics");
	for (double y0 = -charstep; y0 > -2.1; y0 -= charstep) {
		add_characteristic(unioner, y0);
	}
	debug(LOG_DEBUG, DEBUG_LOG, 0, "extract union of characteristics");
	return unioner.get_union();
}

//////////////////////////////////////////////////////////////////////
// build_xcharacteristics implementation
//////////////////////////////////////////////////////////////////////

Nef_polyhedron	build_xcharacteristics() {
	Nef_nary_union	unioner;
	for (double x0 = charstep; a * x0 * x0 < 2; x0 += charstep) {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "characteristic for x0 = %f",
			x0);
		Polyhedron	p;
		double	tmax = x0 * asinh(2 / x0);
		Interval	interval(-tmax - 0.2, tmax + 0.2);
		CharacteristicX	cx(x0, -a * x0 * x0);
		Build_Curve	charcurve(cx, interval, steps, phisteps,
					smallcurveradius);
		p.delegate(charcurve);
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add characteristic for x0 = %f",
			x0);
		unioner.add_polyhedron(p);
	}
	return unioner.get_union();
}

} // namespace csg

