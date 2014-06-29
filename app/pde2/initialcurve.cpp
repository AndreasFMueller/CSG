/*
 * initialcurve.h -- draw the initial curve
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _initialcurve_h
#define _initialcurve_h

#include <common.h>
#include <debug.h>
#include <Curve.h>

namespace csg {

/**
 * \brief Curve function representing Cauchy initial conditions
 */
class Sine : public CurveFunction {
public:
	Sine() : CurveFunction() { }
	virtual point	position(double t) const { return point(0, t, sin(t)); }
	virtual vector	tangent(double t) const { return vector(0, 1, cos(t)); }
	virtual vector	normal(double t) const { return vector::e3; }
};

Nef_polyhedron	build_initialcurve(int curvesteps, int phisteps) {
	debug(LOG_DEBUG, DEBUG_LOG, 0, "adding initial curve");
	Interval	interval(-0.1, M_PI + 0.1);
	Sine		sine;
	Build_Curve	cauchycurve(sine, interval, curvesteps, phisteps, 0.04);
	Polyhedron	p;
	p.delegate(cauchycurve);
	return Nef_polyhedron(p);
}

} // namespace csg

#endif /* _initialcurve_h */
