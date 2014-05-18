/*
 * CartesianCoordinatesSurface.cpp -- Create a surface in cartesian coordinates
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <CartesianCoordinatesSurface.h>

#include <CGAL/Polyhedron_incremental_builder_3.h>

namespace csg {

int	Build_CartesianCoordinatesSurface::vertex(const int x, const int y) const {
	return 2 * (x * (_ysteps + 1) + y);
}

void	Build_CartesianCoordinatesSurface::operator()(Polyhedron::HalfedgeDS& hds) {
	CGAL::Polyhedron_incremental_builder_3<Polyhedron::HalfedgeDS>	B(hds, true);
	B.begin_surface(0, 0, 0);
	double	deltax = _f.xrange().length() / _xsteps;
	double	deltay = _f.yrange().length() / _ysteps;
	int	vertexnumber = 0;
	for (int x = 0; x <= _xsteps; x++) {
		double	_x = _f.xrange().min() + x * deltax;
		for (int y = 0; y <= _ysteps; y++) {
			double	_y = _f.yrange().min() + y * deltay;
			double	z = _f(_x, _y);
			if (debug) {
				fprintf(stderr, "%s:%d: %d (%f,%f,%f)\n",
					__FILE__, __LINE__, vertexnumber++,
					_x, _y, z + _h/2);
				fprintf(stderr, "%s:%d: %d (%f,%f,%f)\n",
					__FILE__, __LINE__, vertexnumber++,
					_x, _y, z - _h/2);
			}
			B.add_vertex(Point(_x, _y, z + _h/2));
			B.add_vertex(Point(_x, _y, z - _h/2));
		}
	}
	int	facenumber = 0;
	for (int x = 0; x < _xsteps; x++) {
		for (int y = 0; y < _ysteps; y++) {
			if (debug) {
				fprintf(stderr, "%s:%d: x = %d, y = %d: %d (%d, %d, %d)\n",
					__FILE__, __LINE__, x, y, facenumber++,
					vertex(x,     y    )    ,
					vertex(x + 1, y    )    ,
					vertex(x,     y + 1)    );
			}
			B.begin_facet();
			B.add_vertex_to_facet(vertex(x,     y    )    );
			B.add_vertex_to_facet(vertex(x + 1, y    )    );
			B.add_vertex_to_facet(vertex(x,     y + 1)    );
			B.end_facet();
			if (debug) {
				fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
					__FILE__, __LINE__, facenumber++,
					vertex(x,     y    ) + 1,
					vertex(x,     y + 1) + 1,
					vertex(x + 1, y    ) + 1);
			}
			B.begin_facet();
			B.add_vertex_to_facet(vertex(x,     y    ) + 1);
			B.add_vertex_to_facet(vertex(x,     y + 1) + 1);
			B.add_vertex_to_facet(vertex(x + 1, y    ) + 1);
			B.end_facet();
			if (debug) {
				fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
					__FILE__, __LINE__, facenumber++,
					vertex(x,     y + 1)    ,
					vertex(x + 1, y    )    ,
					vertex(x + 1, y + 1)    );
			}
			B.begin_facet();
			B.add_vertex_to_facet(vertex(x,     y + 1)    );
			B.add_vertex_to_facet(vertex(x + 1, y    )    );
			B.add_vertex_to_facet(vertex(x + 1, y + 1)    );
			B.end_facet();
			if (debug) {
				fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
					__FILE__, __LINE__, facenumber++,
					vertex(x,     y + 1) + 1,
					vertex(x + 1, y + 1) + 1,
					vertex(x + 1, y    ) + 1);
			}
			B.begin_facet();
			B.add_vertex_to_facet(vertex(x,     y + 1) + 1);
			B.add_vertex_to_facet(vertex(x + 1, y + 1) + 1);
			B.add_vertex_to_facet(vertex(x + 1, y    ) + 1);
			B.end_facet();
		}
	}
	for (int x = 0; x < _xsteps; x++) {
		if (debug) {
			fprintf(stderr, "%s:%d: border x = %d, %d (%d, %d, %d)\n",
				__FILE__, __LINE__, x, facenumber++,
				vertex(x    , 0)    ,
				vertex(x    , 0) + 1,
				vertex(x + 1, 0)    );
		}
		B.begin_facet();
		B.add_vertex_to_facet(vertex(x    , 0)    );
		B.add_vertex_to_facet(vertex(x    , 0) + 1);
		B.add_vertex_to_facet(vertex(x + 1, 0)    );
		B.end_facet();
		if (debug) {
			fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
				__FILE__, __LINE__, facenumber++,
				vertex(x + 1, 0)    ,
				vertex(x    , 0) + 1,
				vertex(x + 1, 0) + 1);
		}
		B.begin_facet();
		B.add_vertex_to_facet(vertex(x + 1, 0)    );
		B.add_vertex_to_facet(vertex(x    , 0) + 1);
		B.add_vertex_to_facet(vertex(x + 1, 0) + 1);
		B.end_facet();
		if (debug) {
			fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
				__FILE__, __LINE__, facenumber++,
				vertex(x    , _ysteps),
				vertex(x + 1, _ysteps),
				vertex(x    , _ysteps) + 1);
		}
		B.begin_facet();
		B.add_vertex_to_facet(vertex(x    , _ysteps));
		B.add_vertex_to_facet(vertex(x + 1, _ysteps));
		B.add_vertex_to_facet(vertex(x    , _ysteps) + 1);
		B.end_facet();
		if (debug) {
			fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
				__FILE__, __LINE__, facenumber++,
				vertex(x + 1, _ysteps),
				vertex(x + 1, _ysteps) + 1,
				vertex(x    , _ysteps) + 1);
		}
		B.begin_facet();
		B.add_vertex_to_facet(vertex(x + 1, _ysteps));
		B.add_vertex_to_facet(vertex(x + 1, _ysteps) + 1);
		B.add_vertex_to_facet(vertex(x    , _ysteps) + 1);
		B.end_facet();
	}
	for (int y = 0; y < _ysteps; y++) {
		if (debug) {
			fprintf(stderr, "%s:%d: border y = %d, %d (%d, %d, %d)\n",
				__FILE__, __LINE__, y, facenumber++,
				vertex(0, y    )    ,
				vertex(0, y + 1)    ,
				vertex(0, y    ) + 1);
		}
		B.begin_facet();
		B.add_vertex_to_facet(vertex(0, y    )    );
		B.add_vertex_to_facet(vertex(0, y + 1)    );
		B.add_vertex_to_facet(vertex(0, y    ) + 1);
		B.end_facet();
		if (debug) {
			fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
				__FILE__, __LINE__, facenumber++,
				vertex(0, y + 1)    ,
				vertex(0, y + 1) + 1,
				vertex(0, y    ) + 1);
		}
		B.begin_facet();
		B.add_vertex_to_facet(vertex(0, y + 1)    );
		B.add_vertex_to_facet(vertex(0, y + 1) + 1);
		B.add_vertex_to_facet(vertex(0, y    ) + 1);
		B.end_facet();
		if (debug) {
			fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
				__FILE__, __LINE__, facenumber++,
				vertex(_xsteps, y    )    ,
				vertex(_xsteps, y    ) + 1,
				vertex(_xsteps, y + 1)    );
		}
		B.begin_facet();
		B.add_vertex_to_facet(vertex(_xsteps, y    )    );
		B.add_vertex_to_facet(vertex(_xsteps, y    ) + 1);
		B.add_vertex_to_facet(vertex(_xsteps, y + 1)    );
		B.end_facet();
		if (debug) {
			fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
				__FILE__, __LINE__, facenumber++,
				vertex(_xsteps, y    ) + 1,
				vertex(_xsteps, y + 1) + 1,
				vertex(_xsteps, y + 1)    );
		}
		B.begin_facet();
		B.add_vertex_to_facet(vertex(_xsteps, y    ) + 1);
		B.add_vertex_to_facet(vertex(_xsteps, y + 1) + 1);
		B.add_vertex_to_facet(vertex(_xsteps, y + 1)    );
		B.end_facet();
	}
	B.end_surface();
}

} // namespace csg
