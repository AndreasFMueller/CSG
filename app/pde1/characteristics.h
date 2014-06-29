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

class CharacteristicY : public CurveFunction {
	double	y0;
public:
	CharacteristicY(const double _y0, double deltat = 0)
		: CurveFunction(deltat), y0(_y0) { }
	virtual point	position(double t) const;
	virtual vector	tangent(double t) const;
	virtual vector	normal(double t) const;
};

class CharacteristicX : public CurveFunction {
	double	x0;
	double	z;
public:
	CharacteristicX(const double _x0, double _z, double deltat = 0)
		: CurveFunction(deltat), x0(_x0), z(_z) { }
	virtual point	position(double t) const;
	virtual vector	tangent(double t) const;
	virtual	vector	normal(double t) const;
};

class Asymptote : public CurveFunction {
	double	m;
public:
	Asymptote(const double _m, double deltat = 0)
		: CurveFunction(deltat), m(_m) { }
	virtual point	position(double t) const;
	virtual vector	tangent(double t) const;
	virtual vector	normal(double t) const;
};

extern Nef_polyhedron	build_xcharacteristics();
extern Nef_polyhedron	build_characteristics();

} // namespace csg

#endif /* _characteristics_h */
