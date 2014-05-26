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
#include <math.h>

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

class point;

class vector {
	double	_x, _y, _z;
public:
	vector() : _x(0), _y(0), _z(0) { }
	vector(double x, double y, double z) : _x(x), _y(y), _z(z) { }
	vector(const point& from, const point& to);
	const double&	x() const { return _x; }
	const double&	y() const { return _y; }
	const double&	z() const { return _z; }
	vector	operator+(const vector& v) const;
	vector	operator-(const vector& v) const;
	vector	operator/(double l) const;
	double	operator*(const vector& v) const;
	double	norm() const;
	vector	normalized() const;
	vector	cross(const vector& v) const;
	static const vector	e1;
	static const vector	e2;
	static const vector	e3;
	friend vector	operator*(double l, const vector& v);
	friend vector	operator*(const vector& v, double l);
	vector	orthogonalto(const vector& v) const;
	bool	parallel(const vector& v) const;
};

vector	operator*(double l, const vector& v);
vector	operator*(const vector& v, double l);

class frame {
	vector	_v1, _v2, _v3;
	void	orthogonalize(const vector& n);
public:
	const vector&	v1() const { return _v1; }
	const vector&	v2() const { return _v2; }
	const vector&	v3() const { return _v3; }
public:
	frame(const vector& t);
	frame(const vector& t, const vector& n);
};

class point {
	double	_x, _y, _z;
public:
	point(double x, double y, double z) : _x(x), _y(y), _z(z) { }
	point() : _x(0), _y(0), _z(0) { }
	const double&	x() const { return _x; }
	const double&	y() const { return _y; }
	const double&	z() const { return _z; }
	point	operator+(const vector& other) const {
		return point(_x + other.x(), _y + other.y(), _z + other.z());
	}
	point	operator-(const vector& other) const {
		return point(_x - other.x(), _y - other.y(), _z - other.z());
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

class PolarDomain {
	Interval	_rrange;
	Interval	_phirange;
public:
	PolarDomain(const Interval& rrange, const Interval& phirange)
		: _rrange(rrange), _phirange(phirange) { }
	const Interval&	rrange() const { return _rrange; }
	const Interval&	phirange() const { return _phirange; }
};

} // namespace csg

#endif /* _common_h */
