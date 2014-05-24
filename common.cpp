/*
 * common.cpp -- common stuff
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <common.h>

int	debug = 0;

namespace csg {

const Interval	Interval2Pi(0, 2 * M_PI);

vector	operator*(double l, const vector& v) {
	return vector(l * v._x, l * v._y, l * v._z);
}

vector	operator*(const vector& v, double l) {
	return vector(l * v._x, l * v._y, l * v._z);
}

vector	vector::operator/(double l) const {
	return vector(x() / l, y() / l, z() / l);
}

double	vector::norm() const {
	return sqrt(_x * _x + _y * _y + _z * _z);
}

double	vector::operator*(const vector& v) const {
	return v.x() * _x + v.y() * _y + v.z() * _z;
}

vector	vector::operator+(const vector& v) const {
	return vector(_x + v.x(), _y + v.y(), _z + v.z());
}

vector	vector::operator-(const vector& v) const {
	return vector(_x - v.x(), _y - v.y(), _z - v.z());
}

vector	vector::cross(const vector& other) const {
	return vector(
		_y * other.z() - _z * other.y(),
		_z * other.x() - _x * other.z(),
		_x * other.y() - _y * other.x()
	);
}

vector::vector(const point& from, const point& to)
	: _x(to.x() - from.x()), _y(to.y() - from.y()), _z(to.z() - from.z()) {
}

const vector	vector::e1(1, 0, 0);
const vector	vector::e2(0, 1, 0);
const vector	vector::e3(0, 0, 1);

void	frame::orthogonalize(const vector& n) {
	_v2 = n - _v1 * (n * _v1);
	if (0 == _v2.norm()) {
		throw std::runtime_error("normal vector has norm 0");
	}
	_v2 = _v2 / _v2.norm();
	_v3 = _v1.cross(_v2);
}

frame::frame(const vector& t) {
	_v1 = t / t.norm();
	try {
		orthogonalize(vector::e1);
	} catch (...) { }
	try {
		orthogonalize(vector::e2);
	} catch (...) { }
	orthogonalize(vector::e3);
}

frame::frame(const vector& t, const vector& n) {
	_v1 = t / t.norm();
	orthogonalize(n);
}

} // namespace csg
