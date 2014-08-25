/*
 * characteristics.h -- classes for characteristics
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _characteristics_h
#define _characteristics_h

#include <common.h>
#include <debug.h>
#include <Cartesian.h>
#include <Curve.h>
#include <math.h>

namespace csg {

class Characteristic : public CurveFunction {
	double	r;
public:
	Characteristic(const double _r, double deltat = 0.0001)
		: CurveFunction(deltat), r(_r) { }
	virtual point	position(double t) const;
	virtual vector	tangent(double t) const;
};

extern Nef_polyhedron	build_characteristics();

} // namespace csg

#endif /* _characteristics_h */
