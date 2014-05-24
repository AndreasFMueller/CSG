/*
 * Arrow.cpp -- create an arrow
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <Arrow.h>

namespace csg {

int	Build_Arrow::vertex(int phi, int z) const {
	return 3 * phi + z + 1;
}

void	Build_Arrow::operator()(Polyhedron::HalfedgeDS& hds) {
	Builder	B(hds, true);
	B.begin_surface(0, 0, 0);

	// initial point
	add_vertex(B, _from);

	// create the frame
	vector	t(_from, _to);
	frame	f(t);
	point	head = _to - (2 * _radius) * f.v1();

	double	deltaphi = 2 * M_PI / _steps;
	for (int phi = 0; phi < _steps; phi++) {
		double	_phi = phi * deltaphi;
		vector	o = _radius * (cos(_phi) * f.v3() + sin(_phi) * f.v2());
		add_vertex(B, _from + o);
		add_vertex(B, head + o);
		add_vertex(B, head + (2 * o));
	}

	// final point
	add_vertex(B, _to);

	// add triangles
	for (int phi = 0; phi < _steps - 1; phi++) {
		// end cap
		add_facet(B,
			0,
			vertex(phi, 0),
			vertex(phi + 1, 0));

		// shaft
		add_facet(B,
			vertex(phi, 0),
			vertex(phi, 1),
			vertex(phi + 1, 0));
		add_facet(B,
			vertex(phi + 1, 0),
			vertex(phi, 1),
			vertex(phi + 1, 1));

		// arrow back side
		add_facet(B,
			vertex(phi, 1),
			vertex(phi, 2),
			vertex(phi + 1, 1));
		add_facet(B,
			vertex(phi + 1, 1),
			vertex(phi, 2),
			vertex(phi + 1, 2));

		// arrow head
		add_facet(B,
			vertex(phi, 2),
			vertexnumber() - 1,
			vertex(phi + 1, 2));
	}

	add_facet(B,
		0,
		vertex(_steps - 1, 0),
		vertex(0, 0));

	add_facet(B,
		vertex(_steps - 1, 0),
		vertex(_steps - 1, 1),
		vertex(0, 0));
	add_facet(B,
		vertex(0, 0),
		vertex(_steps - 1, 1),
		vertex(0, 1));

	// arrow back side
	add_facet(B,
		vertex(_steps - 1, 1),
		vertex(_steps - 1, 2),
		vertex(0, 1));
	add_facet(B,
		vertex(0, 1),
		vertex(_steps - 1, 2),
		vertex(0, 2));

	// arrow head
	add_facet(B,
		vertex(_steps - 1, 2),
		vertexnumber() - 1,
		vertex(0, 2));

	B.end_surface();
}

} // namespace csg
