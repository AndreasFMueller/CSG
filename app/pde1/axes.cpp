/*
 * axes.cpp -- create the axes
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <axes.h>
#include <debug.h>
#include <Arrow.h>
#include <parameters.h>

namespace csg {

Nef_polyhedron	build_axes() {
	Nef_nary_union	unioner;
	{
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add X-axis");
		Polyhedron	p;
		Build_Arrow	b(point(-0.1, 0, 0),
					point(4.1, 0, 0), smallcurveradius, 16);
		p.delegate(b);
		unioner.add_polyhedron(p);
	}
	{
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add Y-axis");
		Polyhedron	p;
		Build_Arrow	b(point(0, -2, 0),
					point(0, 2, 0), smallcurveradius, 16);
		p.delegate(b);
		unioner.add_polyhedron(p);
	}
	{
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add Z-axis");
		Polyhedron	p;
		Build_Arrow	b(point(0, 0, -2),
					point(0, 0, 2), smallcurveradius, 16);
		p.delegate(b);
		unioner.add_polyhedron(p);
	}
	debug(LOG_DEBUG, DEBUG_LOG, 0, "extract axes union");
	return unioner.get_union();
}

} // namespace csg
