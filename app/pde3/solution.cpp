/*
 * solution.cpp -- solution function in polar coordinates
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <solution.h>
#include <math.h>
#include <debug.h>
#include <Polar.h>
#include <Box.h>
#include <parameters.h>

namespace csg {

point	Solution::p(double r, double t) const {
	double	x = -r * cos(t);
	double	y = -r * sin(t);
	double	z = a * ((1 + 0.5 * r * r) * t - 0.25 * r * r * sin(2 * t));
	return point(x, y, z);
}

double	h = 0.0001;

vector	Solution::v(double r, double t) const {
	return (p(r + h, t) - p(r, t)).cross((p(r, t + h) - p(r, t))).normalized();
;
}

Nef_polyhedron	build_solution(double thickness) {
	debug(LOG_DEBUG, DEBUG_LOG, 0, "building solution surface");
	Polyhedron	p;
	PolarDomain	domain(Interval(0.01, 2.1 * sqrt(2)), Interval(-M_PI, M_PI));
	Solution	solution(a);
	Build_PolarPointFunction	s(solution, domain, 2 * steps, 2 * steps, thickness);
	p.delegate(s);
	Nef_polyhedron	surface(p);

	Build_Box	box(point(-2, -2, -2), point(2, 2, 2));
	Polyhedron	boxp;
	boxp.delegate(box);
	debug(LOG_DEBUG, DEBUG_LOG, 0, "intersect with box");
	return surface * boxp;
}

} // namespace csg
