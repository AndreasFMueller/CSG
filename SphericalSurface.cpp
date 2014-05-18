/*
 * SphericalSurface.cpp -- Create a surface in spherical coordinates
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <SphericalSurface.h>

#include <CGAL/Polyhedron_incremental_builder_3.h>

namespace csg {

void	Build_SphericalSurface::operator()(Polyhedron::HalfedgeDS& hds) {
	CGAL::Polyhedron_incremental_builder_3<Polyhedron::HalfedgeDS>	B(hds, true);
	B.begin_surface(0, 0, 0);
	B.add_vertex(Point(0, 0, _f(0, 0)));
	double	delta = M_PI / (2 * _steps);
	int	last = 0;
	for (int theta = 1; theta < 2 * _steps; theta++) {
		double	_theta = theta * delta;
		double	costheta = cos(_theta);
		double	sintheta = sin(_theta);
		for (int phi = 0; phi < 4 * _steps; phi++) {
			double	_phi = phi * delta;
			double	_radius = _f(_theta, _phi);
			double	st = _radius * sintheta;
			double	x = st * cos(_phi);
			double	y = st * sin(_phi);
			double	z = _radius * costheta;
			B.add_vertex(Point(x, y, z));
			last++;
			if (debug) {
				fprintf(stderr, "%s:%d: point %d(%d,%d) = "
					"%f,%f,%f\n", __FILE__, __LINE__,
					last, theta, phi, x, y, z);
			}
		}
	}
	B.add_vertex(Point(0, 0, -_f(M_PI, 0)));
	last++;

	// triangles around the north pole
	if (debug) {
		fprintf(stderr, "%s:%d: triangles around north pole\n",
			__FILE__, __LINE__);
	}
	int	trianglecounter = 0;
	for (int i = 1; i <= 4 * _steps - 1; i++) {
		if (debug) {
			fprintf(stderr, "%s:%d: add triangle %d: "
				"%d, %d, %d\n", __FILE__, __LINE__,
				trianglecounter, 0, i, i + 1);
		}
		B.begin_facet();
		B.add_vertex_to_facet(0);
		B.add_vertex_to_facet(i);
		B.add_vertex_to_facet(i + 1);
		B.end_facet();
		trianglecounter++;
	}
	if (debug) {
		fprintf(stderr, "%s:%d: add triangle: %d: %d, %d, %d\n",
			__FILE__, __LINE__, trianglecounter,
			0, 4 * _steps, 1);
	}
	B.begin_facet();
	B.add_vertex_to_facet(0);
	B.add_vertex_to_facet(4 * _steps);
	B.add_vertex_to_facet(1);
	B.end_facet();
	trianglecounter++;

	// triangles around the south pole
	if (debug) {
		fprintf(stderr, "%s:%d: triangles around south pole\n",
			__FILE__, __LINE__);
	}
	for (int i = 1; i <= 4 * _steps - 1; i++) {
		if (debug) {
			fprintf(stderr, "%s:%d: add triangle %d: "
				"%d, %d, %d\n", __FILE__, __LINE__,
				trianglecounter,
				last,
				last - i,
				last - i - 1);
		}
		B.begin_facet();
		B.add_vertex_to_facet(last);
		B.add_vertex_to_facet(last - i);
		B.add_vertex_to_facet(last - i - 1);
		B.end_facet();
		trianglecounter++;
	}
	if (debug) {
		fprintf(stderr, "%s:%d: add triangle: %d: %d, %d, %d\n",
			__FILE__, __LINE__, trianglecounter,
			last,
			last - 4 * _steps,
			last - 1);
	}
	B.begin_facet();
	B.add_vertex_to_facet(last);
	B.add_vertex_to_facet(last - 4 * _steps);
	B.add_vertex_to_facet(last - 1);
	B.end_facet();
	trianglecounter++;

	// triangles in strips
	for (int theta = 1; theta < 2 * _steps - 1; theta++) {
		if (debug) {
			fprintf(stderr, "%s:%d: zone theta = %d\n",
				__FILE__, __LINE__, theta);
		}
		int	t = 4 * (theta - 1) * _steps + 1;
		for (int phi = 0; phi < 4 * _steps - 1; phi++) {
			if (debug) {
				fprintf(stderr, "%s:%d: quad %d: "
					"(%d, %d, %d), (%d, %d, %d)\n",
					__FILE__, __LINE__,
					trianglecounter,
					t + phi,
					t + phi + 4 * _steps,
					t + phi + 1,
					t + phi + 1,
					t + phi + 4 * _steps,
					t + phi + 4 * _steps + 1);
			}
			B.begin_facet();
			B.add_vertex_to_facet(t + phi);
			B.add_vertex_to_facet(t + phi + 4 * _steps);
			B.add_vertex_to_facet(t + phi + 1);
			B.end_facet();
			trianglecounter++;

			B.begin_facet();
			B.add_vertex_to_facet(t + phi + 1);
			B.add_vertex_to_facet(t + phi + 4 * _steps);
			B.add_vertex_to_facet(t + phi + 4 * _steps + 1);
			B.end_facet();
			trianglecounter++;
		}
		if (debug) {
			fprintf(stderr, "%s:%d: lquad %d: (%d, %d, %d) "
				"(%d, %d, %d)\n",
				__FILE__, __LINE__, trianglecounter,
				t,
				t + 4 * _steps - 1,
				t + 8 * _steps - 1,
				t,
				t + 8 * _steps - 1,
				t + 4 * _steps);
		}
		B.begin_facet();
		B.add_vertex_to_facet(t);
		B.add_vertex_to_facet(t + 4 * _steps - 1);
		B.add_vertex_to_facet(t + 8 * _steps - 1);
		B.end_facet();
		trianglecounter++;

		B.begin_facet();
		B.add_vertex_to_facet(t);
		B.add_vertex_to_facet(t + 8 * _steps - 1);
		B.add_vertex_to_facet(t + 4 * _steps);
		B.end_facet();
		trianglecounter++;
	}
	B.end_surface();
}

} // namespace csg
