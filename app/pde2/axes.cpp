/*
 * axes.h -- create axes
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _axes_h
#define _axes_h

#include <common.h>
#include <Arrow.h>
#include <debug.h>

namespace csg {

Nef_polyhedron	build_axes() {
	Nef_nary_union	unioner;
	// now add the various components, starting with the X-axis
	try {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add X-axis");
		Polyhedron	p;
		Build_Arrow	buildx(point(-0.1, 0, 0),
					point(1.9, 0, 0), 0.02, 16);
		p.delegate(buildx);
		unioner.add_polyhedron(p);
	} catch (std::exception& x) {
		fprintf(stderr, "exception while building x-axis: %s\n",
			x.what());
	} catch (...) {
		fprintf(stderr, "exception while building x-axis\n");
	}

	// adding the Y-axis
	try {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add Y-axis");
		Polyhedron	p;
		Build_Arrow	buildy(point(0, -0.1, 0),
					point(0, M_PI + 0.1, 0), 0.02, 16);
		p.delegate(buildy);
		unioner.add_polyhedron(p);
	} catch (std::exception& x) {
		fprintf(stderr, "exception while building y-axis: %s\n",
			x.what());
	} catch (...) {
		fprintf(stderr, "exception while building y-axis\n");
	}

	// adding the Z-axis
	try {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add Z-axis");
		Polyhedron	p;
		Build_Arrow	buildz(point(0, 0, -0.1), point(0, 0, 1.1),
					0.02, 16);
		p.delegate(buildz);
		unioner.add_polyhedron(p);
	} catch (std::exception& x) {
		fprintf(stderr, "exception while building z-axis: %s\n",
			x.what());
	} catch (...) {
		fprintf(stderr, "exception while building z-axis\n");
	}

	return unioner.get_union();
}

} // namespace csg

#endif /* _axes_h */
