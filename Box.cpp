/*
 * Box.cpp -- build a box
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <Box.h>

namespace csg {

static double	min(double a, double b) {
	if (a < b) {
		return a;
	}
	return b;
}

static double	max(double a, double b) {
	if (a > b) {
		return a;
	}
	return b;
}

void	Build_Box::standardize() {
	double	ax = min(_a.x(), _b.x());
	double	bx = max(_a.x(), _b.x());
	double	ay = min(_a.y(), _b.y());
	double	by = max(_a.y(), _b.y());
	double	az = min(_a.z(), _b.z());
	double	bz = max(_a.z(), _b.z());
	_a = point(ax, ay, az);
	_b = point(bx, by, bz);
}

void	Build_Box::operator()(Polyhedron::HalfedgeDS& hds) {
	Builder	B(hds, true);
	B.begin_surface(0, 0, 0);
	add_vertex(B, _a.x(), _a.y(), _a.z());
	add_vertex(B, _b.x(), _a.y(), _a.z());
	add_vertex(B, _a.x(), _b.y(), _a.z());
	add_vertex(B, _a.x(), _a.y(), _b.z());
	add_vertex(B, _a.x(), _b.y(), _b.z());
	add_vertex(B, _b.x(), _a.y(), _b.z());
	add_vertex(B, _b.x(), _b.y(), _a.z());
	add_vertex(B, _b.x(), _b.y(), _b.z());
	add_facet(B, 0, 1, 3); // front
	add_facet(B, 1, 5, 3);
	add_facet(B, 2, 4, 6); // back
	add_facet(B, 4, 7, 6);
	add_facet(B, 1, 6, 5); // right
	add_facet(B, 6, 7, 5);
	add_facet(B, 0, 3, 2); // left
	add_facet(B, 3, 4, 2);
	add_facet(B, 0, 2, 1); // bottom
	add_facet(B, 2, 6, 1);
	add_facet(B, 3, 5, 4); // top
	add_facet(B, 4, 5, 7);
	B.end_surface();
}

} // namespace csg
