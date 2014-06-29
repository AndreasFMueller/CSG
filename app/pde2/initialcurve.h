/*
 * initialcurve.h -- draw the initial curve
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _initialcurve_h
#define _initialcurve_h

#include <common.h>

namespace csg {

extern Nef_polyhedron	build_initialcurve(int curvesteps, int phisteps);

} // namespace csg

#endif /* _initialcurve_h */
