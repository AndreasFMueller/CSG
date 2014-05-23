/*
 * SphericalSphere.cpp -- Create a spherical mesh based 
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <SphericalSphere.h>

#include <CGAL/Polyhedron_incremental_builder_3.h>

namespace csg {

void	Build_SphericalSphere::operator()(Polyhedron::HalfedgeDS& hds) {
	Builder	B(hds, true);
	B.begin_surface(0, 0, 0);
	add_vertex(B, 0, 0, _radius);
	double	delta = M_PI / (2 * _steps);
	for (int theta = 1; theta < 2 * _steps; theta++) {
		double	st = _radius * sin(theta * delta);
		double	z = _radius * cos(theta * delta);
		for (int phi = 0; phi < 4 * _steps; phi++) {
			double	x = st * cos(phi * delta);
			double	y = st * sin(phi * delta);
			add_vertex(B, x, y, z);
		}
	}
	add_vertex(B, 0, 0, -_radius);

	// triangles around the north pole
	if (debug) {
		fprintf(stderr, "%s:%d: triangles around north pole\n",
			__FILE__, __LINE__);
	}
	for (int i = 1; i <= 4 * _steps - 1; i++) {
		add_facet(B, 0, i, i + 1);
	}
	add_facet(B, 0, 4 * _steps, 1);

	// triangles around the south pole
	if (debug) {
		fprintf(stderr, "%s:%d: triangles around south pole\n",
			__FILE__, __LINE__);
	}
	for (int i = 1; i <= 4 * _steps - 1; i++) {
		add_facet(B,
			vertexnumber() - 1,
			vertexnumber() - i - 1,
			vertexnumber() - i - 2);
	}
	add_facet(B,
		vertexnumber() - 1,
		vertexnumber() - 4 * _steps - 1,
		vertexnumber() - 2);

	// triangles in strips
	for (int theta = 1; theta < 2 * _steps - 1; theta++) {
		if (debug) {
			fprintf(stderr, "%s:%d: zone theta = %d\n",
				__FILE__, __LINE__, theta);
		}
		int	t = 4 * (theta - 1) * _steps + 1;
		for (int phi = 0; phi < 4 * _steps - 1; phi++) {
			add_facet(B,
				t + phi,
				t + phi + 4 * _steps,
				t + phi + 1);
			add_facet(B,
				t + phi + 1,
				t + phi + 4 * _steps,
				t + phi + 4 * _steps + 1);
		}
		add_facet(B,
			t,
			t + 4 * _steps - 1,
			t + 8 * _steps - 1);
		add_facet(B,
			t,
			t + 8 * _steps - 1,
			t + 4 * _steps);
	}
	if (debug) {
		fprintf(stderr, "%s:%d: all triangles added\n",
			__FILE__, __LINE__);
	}
	B.end_surface();
}

} // namespace csg
