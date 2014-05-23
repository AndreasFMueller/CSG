/*
 * Curve.h -- create a tube representation of a curve
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _Curve_h
#define _Curve_h

#include <common.h>
#include <Surface.h>

namespace csg {

/**
 * \brief Base class for curves
 */
class CurveFunction {
public:
	virtual point	operator()(double t) const = 0;
};

/**
 * \brief Class to build space curves
 */
class Curve : public Build_Surface {
	CurveFunction&	_f;
	Interval	_interval;
	int	_steps;
	int	_tsteps;
	double	_r;
public:
	Curve(CurveFunction& f, const Interval& interval,
		int steps, int tsteps, double r)
		: _f(f), _interval(interval),
		_steps(steps), _tsteps(tsteps), _r(r) {
	}
	void	operator()(Polyhedron::HalfedgeDS& hds);
};

} // namespace csg

#endif /* _Curve_h */
