/*
 * common.h -- common definitions we want to use in our tools 
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _common_h
#define _common_h

#include <CGAL/Gmpq.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>

extern int	debug;

namespace csg {

typedef CGAL::Simple_cartesian<CGAL::Gmpq>	Kernel;
typedef CGAL::Polyhedron_3<Kernel>		Polyhedron;
typedef typename Polyhedron::HalfedgeDS::Vertex Vertex;
typedef typename Vertex::Point  		Point;
typedef CGAL::Nef_polyhedron_3<Kernel>		Nef_polyhedron;
typedef Kernel::Aff_transformation_3		Aff_transformation;

class Interval {
	double	_min, _max;
public:
	Interval(double min, double max) : _min(min), _max(max) { }
	const double&	min() const { return _min; }
	const double&	max() const { return _max; }
	const double	length() const { return _max - _min; }
	bool	operator==(const Interval& other) const {
		return (_min == other.min()) && (_max == other.max());
	}
};

extern const Interval	Interval2Pi;

class CartesianFunction {
	Interval	_xrange;
	Interval	_yrange;
public:
	CartesianFunction(const Interval& xrange, const Interval& yrange)
		: _xrange(xrange), _yrange(yrange) { }
	virtual double	operator()(const double x, const double y) = 0;
	const Interval& xrange() const { return _xrange; }
	const Interval&	yrange() const { return _yrange; }
};

class PolarFunction {
	Interval	_rrange;
	Interval	_phirange;
public:
	PolarFunction(const Interval& rrange, const Interval& phirange)
		: _rrange(rrange), _phirange(phirange) { }
	virtual double	operator()(const double r, const double phi) = 0;
	const Interval&	rrange() const { return _rrange; }
	const Interval&	phirange() const { return _phirange; }
};

} // namespace csg

#endif /* _common_h */
