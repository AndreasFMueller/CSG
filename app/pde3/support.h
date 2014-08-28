/*
 * support.h -- 
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _support_h
#define _support_h

#include <common.h>
#include <Cartesian.h>
#include <hyperbola.h>
#include <parameters.h>

namespace csg {

class axessupport {
};

class cutsupport : public PointFunction {
	double	_y;
	int	_sign;
	double	top(double x) const;
	double	bottom(double x) const;
	hyperbola	h;
public:
	cutsupport(double y, int sign = 1) : _y(y), _sign(sign),
		h(point2(0, a), point2(2, 0)) { }
	virtual point	p(double x, double z) const;
	virtual vector	v(double x, double z) const;
};

Nef_polyhedron	build_cutsupport(double thickness);
Nef_polyhedron	build_axessupport(double thickness);

} // namespace csg

#endif /* _support_h */ 
