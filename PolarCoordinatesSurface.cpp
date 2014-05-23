/*
 * PolarCoordinatesSurface.cpp -- Create a surface in polar coordinates
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <PolarCoordinatesSurface.h>

#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <math.h>

namespace csg {

/**
 * \brief Find out whether the surface is closed
 */
bool	Build_Polar::closed() const {
	return (_domain.phirange() == Interval2Pi) ? true : false;
}

/**
 * \brief Find out whether r=0 is included in the domain
 */
bool	Build_Polar::contains0() const {
	return (_domain.rrange().min() == 0) ? true : false;
}

/**
 * \brief Vertex number for open surface
 */
int	Build_Polar::vertex(const int r, const int phi) const {
	if (closed()) {
		int	p = phi;
		if (phi == _phisteps) {
			p = 0;
		}
		return 2 * (r * _phisteps + p);
	}
	return 2 * (r * (_phisteps + 1) + phi);
}

/**
 * \brief add vertices (common to all surfaces)
 */
void	Build_Polar::add_vertices(Builder& B) {
	if (debug) {
		fprintf(stderr, "%s:%d: vertices\n", __FILE__, __LINE__);
	}
	int	philimit = closed() ? _phisteps : (_phisteps + 1);
	if (debug) {
		fprintf(stderr, "%s:%d: philimit: %d (_phisteps = %d)\n",
			__FILE__, __LINE__, philimit, _phisteps);
	}
	int	rinit = (contains0()) ? 1 : 0;
	for (int r = rinit; r <= _rsteps; r++) {
		double	_r = _domain.rrange().min() + r * deltar;
		if (debug) {
			fprintf(stderr, "%s:%d: r = %d, _r = %f\n",
				__FILE__, __LINE__, r, _r);
		}
		for (int phi = 0; phi < philimit; phi++) {
			double	_phi = _domain.phirange().min() + phi * deltaphi;
			add_vertex(B, p(_r, _phi, _h));
			add_vertex(B, p(_r, _phi, -_h));
		}
	}
	if (contains0()) {
		add_vertex(B, p(0, 0, _h));
		add_vertex(B, p(0, 0, -_h));
	}
}

/**
 * \brief add surface triangles
 *
 * This method does not add the triangles along the border of the domain
 */
void	Build_Polar::add_surface_triangles(Builder& B) {
	if (debug) {
		fprintf(stderr, "%s:%d: surface facets\n", __FILE__, __LINE__);
	}
	int	rlimit = _rsteps - ((contains0()) ? 1 : 0);
	for (int r = 0; r < rlimit; r++) {
		for (int phi = 0; phi < _phisteps; phi++) {
			add_facet(B,
				vertex(r    , phi    ),
				vertex(r + 1, phi    ),
				vertex(r    , phi + 1));

			add_facet(B,
				vertex(r + 1, phi    )    ,
				vertex(r + 1, phi + 1)    ,
				vertex(r    , phi + 1)    );

			add_facet(B,
				vertex(r    , phi    ) + 1,
				vertex(r    , phi + 1) + 1,
				vertex(r + 1, phi    ) + 1);

			add_facet(B,
				vertex(r + 1, phi    ) + 1,
				vertex(r    , phi + 1) + 1,
				vertex(r + 1, phi + 1) + 1);
		}
	}
}

/**
 * \brief add fan triangles
 */
void	Build_Polar::add_surface_fan(Builder& B) {
	if (!contains0()) {
		if (debug) {
			fprintf(stderr, "%s:%d: fan not needed\n",
				__FILE__, __LINE__);
		}
		return;
	}
	if (debug) {
		fprintf(stderr, "%s:%d: add fan\n", __FILE__, __LINE__);
	}
	for (int phi = 0; phi < _phisteps; phi++) {
		add_facet(B,
			vertexnumber() - 2,
			vertex(0, phi),
			vertex(0, phi + 1));
		add_facet(B,
			vertexnumber() - 1,
			vertex(0, phi + 1) + 1,
			vertex(0, phi) + 1);
	}
}

/**
 * \brief add the facets along the radial borders
 */
void	Build_Polar::add_radius_surface(Builder& B) {
	if (closed()) {
		if (debug) {
			fprintf(stderr, "%s:%d: radius surfaces not needed\n",
				__FILE__, __LINE__);
		}
		return;
	}
	if (debug) {
		fprintf(stderr, "%s:%d: radius surfaces\n",
			__FILE__, __LINE__);
	}
	int	rlimit = _rsteps - ((contains0()) ? 1 : 0);
	for (int r = 0; r < rlimit; r++) {
		add_facet(B,
			vertex(r    , 0    )    ,
			vertex(r    , 0    ) + 1,
			vertex(r + 1, 0    )    );
		add_facet(B,
			vertex(r    , 0    ) + 1,
			vertex(r + 1, 0    ) + 1,
			vertex(r + 1, 0    )    );
	}
	if (contains0()) {
		add_facet(B,
			vertex(0, 0) + 1,
			vertex(0, 0)    ,
			vertexnumber() - 2);
		add_facet(B,
			vertexnumber() - 1,
			vertex(0, 0) + 1,
			vertexnumber() - 2);
	}
	for (int r = 0; r < rlimit; r++) {
		add_facet(B,
			vertex(r    , _phisteps)    ,
			vertex(r + 1, _phisteps)    ,
			vertex(r    , _phisteps) + 1);
		add_facet(B,
			vertex(r + 1, _phisteps)    ,
			vertex(r + 1, _phisteps) + 1,
			vertex(r    , _phisteps) + 1);
	}
	if(contains0()) {
		add_facet(B,
			vertex(0, _phisteps)    ,
			vertex(0, _phisteps) + 1,
			vertexnumber() - 2);
		add_facet(B,
			vertex(0, _phisteps) + 1,
			vertexnumber() - 1,
			vertexnumber() - 2);
	}
}

/**
 * \brief add the inner and outer borders
 */
void	Build_Polar::add_perimeter(Builder& B) {
	int	rimindex = _rsteps - ((contains0()) ? 1 : 0);
	if (debug) {
		fprintf(stderr, "%s:%d: outer surface\n", __FILE__, __LINE__);
	}
	for (int phi = 0; phi < _phisteps; phi++) {
		add_facet(B,
			vertex(rimindex, phi    )    ,
			vertex(rimindex, phi    ) + 1,
			vertex(rimindex, phi + 1) + 1);

		add_facet(B,
			vertex(rimindex, phi    )    ,
			vertex(rimindex, phi + 1) + 1,
			vertex(rimindex, phi + 1)    );
	}

	if (!contains0()) {
		if (debug) {
			fprintf(stderr, "%s:%d: inner surface\n",
				__FILE__, __LINE__);
		}
		for (int phi = 0; phi < _phisteps; phi++) {
			add_facet(B,
				vertex(0, phi    )    ,
				vertex(0, phi + 1)    ,
				vertex(0, phi    ) + 1);

			add_facet(B,
				vertex(0, phi + 1)    ,
				vertex(0, phi + 1) + 1,
				vertex(0, phi    ) + 1);
		}
	}
}

/**
 * \brief create the polyhedron
 */
void	Build_Polar::operator()(Polyhedron::HalfedgeDS& hds) {
	Builder	B(hds, true);
	if (debug) {
		fprintf(stderr, "%s:%d: start building surface\n",
			__FILE__, __LINE__);
	}
	B.begin_surface(0, 0, 0);
	add_vertices(B);
	add_surface_triangles(B);
	add_surface_fan(B);
	add_radius_surface(B);
	add_perimeter(B);
	if (debug) {
		fprintf(stderr, "%s:%d: all factes added\n",
			__FILE__, __LINE__);
	}
	B.end_surface();
}

/**
 * \brief Get the point with parameter values r, phi and h
 */
point	Build_PolarFunction::p(double r, double phi, double h) {
	double	x = r * cos(phi);
	double	y = r * sin(phi);
	double	z = _f(r, phi) + (h/2);
	return point(x, y, z);
}

/**
 * \brief Get the point with parameter values r, phi and h
 */
point	Build_PolarPointFunction::p(double r, double phi, double h) {
	return _f.p(r, phi) + _f.v(r, phi) * (h/2);
}

} // namespace csg
