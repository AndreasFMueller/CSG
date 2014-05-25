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
	double	_deltat;
public:
	CurveFunction(double deltat = 0) : _deltat(deltat) { }

	const double&	deltat() const { return _deltat; }
	void	deltat(double d) { _deltat = d; }

	virtual point	position(double t) const = 0;

	virtual vector	tangent(double t) const;
	virtual vector	normal(double t) const;
	frame	frenetframe(double t) const;
};

/**
 * \brief Class to build space curves
 */
class Build_Curve : public Build_Surface {
	CurveFunction&	_f;
	Interval	_interval;
	int	_steps;
	int	_phisteps;
	double	_r;
	int	vertex(int t, int phi) const;
public:
	Build_Curve(CurveFunction& f, const Interval& interval,
		int steps, int phisteps, double r)
		: _f(f), _interval(interval),
		_steps(steps), _phisteps(phisteps), _r(r) {
	}
	void	operator()(Polyhedron::HalfedgeDS& hds);
};

} // namespace csg

#endif /* _Curve_h */
