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
#include <CGAL/Polyhedron_incremental_builder_3.h>

extern int	debug;

namespace csg {

typedef CGAL::Simple_cartesian<CGAL::Gmpq>	Kernel;
typedef CGAL::Polyhedron_3<Kernel>		Polyhedron;
typedef typename Polyhedron::HalfedgeDS::Vertex Vertex;
typedef typename Vertex::Point  		Point;
typedef CGAL::Nef_polyhedron_3<Kernel>		Nef_polyhedron;
typedef Kernel::Aff_transformation_3		Aff_transformation;
typedef CGAL::Polyhedron_incremental_builder_3<Polyhedron::HalfedgeDS>	Builder;

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
	bool	operator!=(const Interval& other) const {
		return !(*this == other);
	}
};

extern const Interval	Interval2Pi;

class Function {
public:
	virtual double	operator()(const double, const double) = 0;
};

class vector {
	double	_x, _y, _z;
public:
	vector(double x, double y, double z) : _x(x), _y(y), _z(z) { }
	const double&	x() const { return _x; }
	const double&	y() const { return _y; }
	const double&	z() const { return _z; }
	vector	operator*(double l) {
		return vector(l * x(), l * y(), l * z());
	}
};

class point {
	double	_x, _y, _z;
public:
	point(double x, double y, double z) : _x(x), _y(y), _z(z) { }
	const double&	x() const { return _x; }
	const double&	y() const { return _y; }
	const double&	z() const { return _z; }
	point	operator+(const vector& other) {
		return point(_x + other.x(), _y + other.y(), _z + other.z());
	}
};

class PointFunction {
public:
	virtual	point	p(double, double) const = 0;
	virtual vector	v(double, double) const = 0;
};

class point_z : public PointFunction {
	Function&	_f;
public:
	point_z(Function& f) : _f(f) { }
	point	p(double x, double y) const {
		return point(x, y, _f(x, y));
	}
	vector	v() const {
		return vector(0, 0, 1);
	}
};

class CartesianDomain {
	Interval	_xrange;
	Interval	_yrange;
public:
	CartesianDomain(const Interval& xrange, const Interval& yrange)
		: _xrange(xrange), _yrange(yrange) { }
	const Interval& xrange() const { return _xrange; }
	const Interval&	yrange() const { return _yrange; }
};

class CartesianFunction : public Function, public CartesianDomain {
public:
	CartesianFunction(const Interval& xrange, const Interval& yrange)
		: CartesianDomain(xrange, yrange) { }
};

class CartesianPointFunction : public PointFunction, public CartesianDomain {
public:
	CartesianPointFunction(const Interval& xrange, const Interval& yrange)
		: CartesianDomain(xrange, yrange) { }
};

class PolarDomain {
	Interval	_rrange;
	Interval	_phirange;
public:
	PolarDomain(const Interval& rrange, const Interval& phirange)
		: _rrange(rrange), _phirange(phirange) { }
	const Interval&	rrange() const { return _rrange; }
	const Interval&	phirange() const { return _phirange; }
};

class PolarFunction : public Function, public PolarDomain {
public:
	PolarFunction(const Interval& rrange, const Interval& phirange)
		: PolarDomain(rrange, phirange) { }
};

class PolarPointFunction : public PointFunction, public PolarDomain {
public:
	PolarPointFunction(const Interval& rrange, const Interval& phirange)
		: PolarDomain(rrange, phirange) { }
};

} // namespace csg

#endif /* _common_h */
