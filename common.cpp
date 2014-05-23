/*
 * common.cpp -- common stuff
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <common.h>

int	debug = 0;

namespace csg {

const Interval	Interval2Pi(0, 2 * M_PI);

vector	vector::operator*(double l) const {
	return vector(l * x(), l * y(), l * z());
}

double	vector::norm() const {
	return sqrt(_x * _x + _y * _y + _z * _z);
}

double	vector::operator*(const vector& v) const {
	return v.x() * _x + v.y() * _y + v.z() * _z;
}

vector	vector::cross(const vector& other) const {
	return vector(
		_y * other.z() - _z * other.y(),
		_z * other.x() - _x * other.z(),
		_x * other.y() - _y * other.x()
	);
}

} // namespace csg
