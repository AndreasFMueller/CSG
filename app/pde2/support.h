/*
 * support.h -- support structure, to make the model more robust
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _support_h
#define _support_h

#include <common.h>

namespace csg {

Nef_polyhedron	build_support(double thickness);

} // namespace csg

#endif /* _support_h */
