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
#include <Box.h>

namespace csg {

point	Characteristic::position(double t) const {
	point	p(-r * cos(t), -r * sin(t), f(r) * t);
	return p;
}

vector	Characteristic::tangent(double t) const {
	vector	v(r * sin(t), -r * cos(t), f(r));
	return v;
}

static void	add_characteristic(Nef_nary_union& unioner, double r) {
	debug(LOG_DEBUG, DEBUG_LOG, 0, "add characteristic for r = %f", r);
	Interval	interval(-M_PI - 0.01, M_PI + 0.01);
	Characteristic	cs(r);
	Build_Curve	charcurve(cs, interval, 3 * steps, 12,
				smallcurveradius);
	Polyhedron	p;
	p.delegate(charcurve);
	Nef_polyhedron	n(p);
	unioner.add_polyhedron(n);
	debug(LOG_DEBUG, DEBUG_LOG, 0, "curve added");
}

Nef_polyhedron	build_characteristics() {
	Nef_nary_union	unioner;
	debug(LOG_DEBUG, DEBUG_LOG, 0, "add characteristics");
	for (double r = 2.5; r > 0.4; r -= 0.5) {
		add_characteristic(unioner, r);
	}
	debug(LOG_DEBUG, DEBUG_LOG, 0, "extract union of characteristics");
	Nef_polyhedron	characteristics = unioner.get_union();

	Build_Box	box(point(-2, -2, -2), point(2, 2, 2));
	Polyhedron	boxp;
	boxp.delegate(box);
	debug(LOG_DEBUG, DEBUG_LOG, 0, "intersect with box");
	return characteristics * boxp;
}

} // namespace csg

