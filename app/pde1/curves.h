/*
 * curves.h -- initial curve and such
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _curves_h
#define _curves_h

#include <common.h>
#include <Cartesian.h>
#include <Curve.h>

namespace csg {

class InitialCurve : public CurveFunction {
public:
	virtual point	position(double t) const;
	virtual vector	tangent(double t) const;
	virtual vector	normal(double t) const;
};

extern Nef_polyhedron	build_initialcurve();

class FCurve : public CurveFunction {
public:
	virtual point	position(double t) const;
	virtual vector	tangent(double t) const;
	virtual vector	normal(double t) const;
};

extern Nef_polyhedron	build_fcurve();

}

#endif /* _curves_h */
