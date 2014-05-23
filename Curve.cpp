/*
 * Curve.cpp -- create a cube representation of a curve
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <Curve.h>

namespace csg {

/**
 * \brief Main function to create surface corresponding to space curve
 */
void	Curve::operator()(Polyhedron::HalfedgeDS& hds) {
	Builder	B(hds, true);
	B.begin_surface(0, 0, 0);
	// add all vertices
	// initial vertex
	add_vertex(B, _f(_interval.min()));

	// intermediate points
	double	deltat = _interval.length() / _steps;
	for (int t = 0; t <= _steps; t++) {
		double	_t = _interval.min() + t * deltat;
		point	w = _f(_t);
	}

	add_vertex(B, _f(_interval.max()));

	// add all facets

	// that's it, we are done
	B.end_surface();
}

} // namespace csg
