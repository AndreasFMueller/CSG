/*
 * PolarCoordinatesSurface.cpp -- Create a surface in polar coordinates
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <PolarCoordinatesSurface.h>

#include <CGAL/Polyhedron_incremental_builder_3.h>

namespace csg {

/**
 * \brief Vertex number for open0 surface
 */
int	Build_PolarCoordinatesSurface::open0_vertex(const int r, const int phi) const {
	return 2 * (r * (_phisteps + 1) + phi);
}

/**
 * \brief Vertex number for open surface
 */
int	Build_PolarCoordinatesSurface::open_vertex(const int r, const int phi) const {
	return 2 * (r * (_phisteps + 1) + phi);
}

/**
 * \brief Vertex number for closed0 surface
 */
int	Build_PolarCoordinatesSurface::closed0_vertex(const int r, const int phi) const {
	return 0;
}

/**
 * \brief Vertex number for closed surface
 */
int	Build_PolarCoordinatesSurface::closed_vertex(const int r, const int phi) const {
	return 0;
}

/**
 * \brief create a closed0 surface
 */
void	Build_PolarCoordinatesSurface::closed0_surface(Polyhedron::HalfedgeDS& hds) {
	
}

/**
 * \brief create a closed surface
 */
void	Build_PolarCoordinatesSurface::closed_surface(Polyhedron::HalfedgeDS& hds) {
	
}

/**
 * \brief create an open0 surface
 */
void	Build_PolarCoordinatesSurface::open0_surface(Polyhedron::HalfedgeDS& hds) {
}

/**
 * \brief create an open surface
 */
void	Build_PolarCoordinatesSurface::open_surface(Polyhedron::HalfedgeDS& hds) {
	CGAL::Polyhedron_incremental_builder_3<Polyhedron::HalfedgeDS>	B(hds, true);
	B.begin_surface(0, 0, 0);
	double	deltar = _f.rrange().length() / _rsteps;
	double	deltaphi = _f.phirange().length() / _phisteps;
	int	vertexnumber = 0;
	if (debug) {
		fprintf(stderr, "%s:%d: vertices\n", __FILE__, __LINE__);
	}
	for (int r = 0; r <= _rsteps; r++) {
		double	_r = _f.rrange().min() + r * deltar;
		for (int phi = 0; phi <= _phisteps; phi++) {
			double	_phi = _f.phirange().min() + phi * deltaphi;
			double	_x = _r * cos(_phi);
			double	_y = _r * sin(_phi);
			double	z = _f(_r, _phi);
			if (debug) {
				fprintf(stderr, "%s:%d: %d (%f,%f,%f)\n",
					__FILE__, __LINE__, vertexnumber,
					_x, _y, z + _h/2);
				fprintf(stderr, "%s:%d: %d (%f,%f,%f)\n",
					__FILE__, __LINE__, vertexnumber + 1,
					_x, _y, z - _h/2);
			}
			B.add_vertex(Point(_x, _y, z + _h/2));
			B.add_vertex(Point(_x, _y, z - _h/2));
			vertexnumber += 2;
		}
	}
	int	facenumber = 0;
	if (debug) {
		fprintf(stderr, "%s:%d: surface facets\n", __FILE__, __LINE__);
	}
	for (int r = 0; r < _rsteps; r++) {
		for (int phi = 0; phi < _phisteps; phi++) {
			if (debug) {
				fprintf(stderr, "%s:%d: r = %d, phi = %d\n",
					__FILE__, __LINE__, r, phi);
				fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
					__FILE__, __LINE__, facenumber,
					open_vertex(r    , phi    ),
					open_vertex(r + 1, phi    ),
					open_vertex(r    , phi + 1));
			}
			B.begin_facet();
			B.add_vertex_to_facet(open_vertex(r    , phi    )    );
			B.add_vertex_to_facet(open_vertex(r + 1, phi    )    );
			B.add_vertex_to_facet(open_vertex(r    , phi + 1)    );
			B.end_facet();
			facenumber++;

			if (debug) {
				fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
					__FILE__, __LINE__, facenumber,
					open_vertex(r + 1, phi    )    ,
					open_vertex(r + 1, phi + 1)    ,
					open_vertex(r    , phi + 1)    );
			}
			B.begin_facet();
			B.add_vertex_to_facet(open_vertex(r + 1, phi    )    );
			B.add_vertex_to_facet(open_vertex(r + 1, phi + 1)    );
			B.add_vertex_to_facet(open_vertex(r    , phi + 1)    );
			B.end_facet();
			facenumber++;

			if (debug) {
				fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
					__FILE__, __LINE__, facenumber,
					open_vertex(r    , phi    ) + 1,
					open_vertex(r    , phi + 1) + 1,
					open_vertex(r + 1, phi    ) + 1);
			}
			B.begin_facet();
			B.add_vertex_to_facet(open_vertex(r    , phi    ) + 1);
			B.add_vertex_to_facet(open_vertex(r    , phi + 1) + 1);
			B.add_vertex_to_facet(open_vertex(r + 1, phi    ) + 1);
			B.end_facet();
			facenumber++;

			if (debug) {
				fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
					__FILE__, __LINE__, facenumber,
				open_vertex(r + 1, phi    ) + 1,
				open_vertex(r    , phi + 1) + 1,
				open_vertex(r + 1, phi + 1) + 1);
			}
			B.begin_facet();
			B.add_vertex_to_facet(open_vertex(r + 1, phi    ) + 1);
			B.add_vertex_to_facet(open_vertex(r    , phi + 1) + 1);
			B.add_vertex_to_facet(open_vertex(r + 1, phi + 1) + 1);
			B.end_facet();
			facenumber++;
		}
	}
	if (debug) {
		fprintf(stderr, "%s:%d: radiant surfaces\n",
			__FILE__, __LINE__);
	}
	for (int r = 0; r < _rsteps; r++) {
		if (debug) {
			fprintf(stderr, "%s:%d: r = %d\n",
				__FILE__, __LINE__, r);
			fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
				__FILE__, __LINE__, facenumber,
				open_vertex(r    , 0    )    ,
				open_vertex(r    , 0    ) + 1,
				open_vertex(r + 1, 0    )    );
		}
		B.begin_facet();
		B.add_vertex_to_facet(open_vertex(r    , 0    )    );
		B.add_vertex_to_facet(open_vertex(r    , 0    ) + 1);
		B.add_vertex_to_facet(open_vertex(r + 1, 0    )    );
		B.end_facet();
		facenumber++;
		if (debug) {
			fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
				__FILE__, __LINE__, facenumber,
				open_vertex(r    , 0    ) + 1,
				open_vertex(r + 1, 0    ) + 1,
				open_vertex(r + 1, 0    )    );
		}
		B.begin_facet();
		B.add_vertex_to_facet(open_vertex(r    , 0    ) + 1);
		B.add_vertex_to_facet(open_vertex(r + 1, 0    ) + 1);
		B.add_vertex_to_facet(open_vertex(r + 1, 0    )    );
		B.end_facet();
		facenumber++;
		if (debug) {
			fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
				__FILE__, __LINE__, facenumber,
				open_vertex(r    , _phisteps)    ,
				open_vertex(r + 1, _phisteps)    ,
				open_vertex(r    , _phisteps) + 1);
		}
		B.begin_facet();
		B.add_vertex_to_facet(open_vertex(r    , _phisteps)    );
		B.add_vertex_to_facet(open_vertex(r + 1, _phisteps)    );
		B.add_vertex_to_facet(open_vertex(r    , _phisteps) + 1);
		B.end_facet();
		facenumber++;
		if (debug) {
			fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
				__FILE__, __LINE__, facenumber,
				open_vertex(r + 1, _phisteps)    ,
				open_vertex(r + 1, _phisteps) + 1,
				open_vertex(r    , _phisteps) + 1);
		}
		B.begin_facet();
		B.add_vertex_to_facet(open_vertex(r + 1, _phisteps)    );
		B.add_vertex_to_facet(open_vertex(r + 1, _phisteps) + 1);
		B.add_vertex_to_facet(open_vertex(r    , _phisteps) + 1);
		B.end_facet();
		facenumber++;
	}
	if (debug) {
		fprintf(stderr, "%s:%d: outer surface\n", __FILE__, __LINE__);
	}
	for (int phi = 0; phi < _phisteps; phi++) {
		if (debug) {
			fprintf(stderr, "%s:%d: phi = %d\n",
				__FILE__, __LINE__, phi);
			fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
				__FILE__, __LINE__, facenumber,
				open_vertex(_rsteps, phi    )    ,
				open_vertex(_rsteps, phi    ) + 1,
				open_vertex(_rsteps, phi + 1) + 1);
		}
		B.begin_facet();
		B.add_vertex_to_facet(open_vertex(_rsteps, phi    )    );
		B.add_vertex_to_facet(open_vertex(_rsteps, phi    ) + 1);
		B.add_vertex_to_facet(open_vertex(_rsteps, phi + 1) + 1);
		B.end_facet();
		facenumber++;

		if (debug) {
			fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
				__FILE__, __LINE__, facenumber,
				open_vertex(_rsteps, phi    )    ,
				open_vertex(_rsteps, phi + 1) + 1,
				open_vertex(_rsteps, phi + 1)    );
		}
		B.begin_facet();
		B.add_vertex_to_facet(open_vertex(_rsteps, phi    )    );
		B.add_vertex_to_facet(open_vertex(_rsteps, phi + 1) + 1);
		B.add_vertex_to_facet(open_vertex(_rsteps, phi + 1)    );
		B.end_facet();
		facenumber++;

		if (debug) {
			fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
				__FILE__, __LINE__, facenumber,
				open_vertex(0, phi    )    ,
				open_vertex(0, phi + 1)    ,
				open_vertex(0, phi    ) + 1);
		}
		B.begin_facet();
		B.add_vertex_to_facet(open_vertex(0, phi    )    );
		B.add_vertex_to_facet(open_vertex(0, phi + 1)    );
		B.add_vertex_to_facet(open_vertex(0, phi    ) + 1);
		B.end_facet();
		facenumber++;

		if (debug) {
			fprintf(stderr, "%s:%d: %d (%d, %d, %d)\n",
				__FILE__, __LINE__, facenumber,
				open_vertex(0, phi + 1)    ,
				open_vertex(0, phi + 1) + 1,
				open_vertex(0, phi    ) + 1);
		}
		B.begin_facet();
		B.add_vertex_to_facet(open_vertex(0, phi + 1)    );
		B.add_vertex_to_facet(open_vertex(0, phi + 1) + 1);
		B.add_vertex_to_facet(open_vertex(0, phi    ) + 1);
		B.end_facet();
		facenumber++;
	}
	B.end_surface();
}

void	Build_PolarCoordinatesSurface::operator()(Polyhedron::HalfedgeDS& hds) {
	if (_f.phirange() == Interval2Pi) {
		if (_f.rrange().min() == 0) {
			return closed0_surface(hds);
		} else {
			return closed_surface(hds);
		}
	} else {
		if (_f.rrange().min() == 0) {
			return open0_surface(hds);
		} else {
			return open_surface(hds);
		}
	}
}

} // namespace csg
