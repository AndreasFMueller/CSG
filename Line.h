/*
 * Line.h -- straight line visualization using the Curve methods
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _Line_h
#define _Line_h

#include <common.h>
#include <Curve.h>

namespace csg {

class Line : public CurveFunction {
	point	_p0;
	vector	_r;
public:
	Line(const point& from, const point& to)
		: CurveFunction(), _p0(from), _r(from, to) { }
	Line(const point& p0, const vector& r)
		: CurveFunction(), _p0(p0), _r(r) { }
	virtual point	position(double t) const { return _p0 + t * _r; }
	virtual vector	tangent(double t) const { return _r; }
	virtual vector	normal(double t) const;
};

class Build_Line : public Build_Curve {
public:
	Build_Line(Line& line, const Interval& interval, int phisteps, double r)
		: Build_Curve(line, interval, 1, phisteps, r) {
	}
};

} // namespace csg

#endif /* _Line_h */
