/*
 * characteristics.cpp -- displaying the characteristics
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _characteristics_h
#define _characteristics_h

#include <characteristics.h>
#include <Curve.h>
#include <Box.h>
#include <debug.h>

namespace csg {

/**
 * \brief Curve function class representing the characteristics
 */
class Characteristic : public CurveFunction {
	double	_y0, _z0;
public:
	Characteristic(double y0, double z0) : _y0(y0), _z0(z0) { }
	virtual point	position(double t) const {
		return point(t, 0.5 * t * t + _y0, _z0 * exp(-t));
	}
	virtual vector	tangent(double t) const {
		return vector(1, t, -_z0 * exp(-t));
	}
	virtual vector normal(double t) const {
		return vector(0, 1, _z0 * exp(-t));
	}
};

Nef_polyhedron	build_characteristics(int phisteps, int curvesteps) {
	debug(LOG_DEBUG, DEBUG_LOG, 0, "adding characteristics");
	// the charcurves Nef_polyhedron is used to accumulate
	// the characteristic curves
	Nef_nary_union	unioner;
	Interval	interval(0, 1.9);
	for (double y0 = -M_PI / 2; y0 <= M_PI + 0.01;
		y0 += M_PI / 8) {
		debug(LOG_DEBUG, DEBUG_LOG, 0,
			"add characteristic for y0 = %f", y0);
		// build a characteristic curve vor a particular
		// y0 vaule
		Characteristic	characteristic(y0, sin(y0));
		Build_Curve	charcurve(characteristic, interval,
					curvesteps, phisteps, 0.03);
		Polyhedron	p;
		p.delegate(charcurve);
		Nef_polyhedron	n(p);

		// add the curve to the union
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add curve to union");
		unioner.add_polyhedron(n);
	}

	// add the curve to the accumulator
	debug(LOG_DEBUG, DEBUG_LOG, 0, "extract union");
	Nef_polyhedron	charcurves = unioner.get_union();
	return charcurves;
}

} // namespace csg

#endif /* _characteristics_h */

