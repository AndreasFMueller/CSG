/*
 * Curve.cpp -- create a cube representation of a curve
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <Curve.h>

namespace csg {

vector  CurveFunction::tangent(double t) const {
	vector  result(position(t), position(t + _deltat));
	return result.normalized();
}

vector  CurveFunction::normal(double t) const {
	vector  T = tangent(t);
	vector  result = tangent(t + _deltat) - T;
	return result.orthogonalto(T).normalized();
}

frame	CurveFunction::frenetframe(double t) const {
	return frame(tangent(t), normal(t));
}

int	Build_Curve::vertex(int t, int phi) const {
	return t * _phisteps + phi + 1;
}

/**
 * \brief Main function to create surface corresponding to space curve
 */
void	Build_Curve::operator()(Polyhedron::HalfedgeDS& hds) {
	Builder	B(hds, true);
	B.begin_surface(0, 0, 0);
	// add all vertices
	// initial vertex
	add_vertex(B, _f.position(_interval.min()));

	// intermediate points
	double	deltat = _interval.length() / _steps;
	double	deltaphi = 2 * M_PI / _phisteps;
	for (int t = 0; t <= _steps; t++) {
		double	_t = _interval.min() + t * deltat;
		if (debug) {
			fprintf(stderr, "%s:%d: circle at t = %f\n", 
				__FILE__, __LINE__, _t);
		}
		point	w = _f.position(_t);
		frame	fr = _f.frenetframe(_t);
		if (debug) {
			fprintf(stderr,
				"%s:%d: tangent (%f, %f, %f), "
				"normal (%f, %f, %f)\n", __FILE__, __LINE__,
				fr.v1().x(), fr.v1().y(), fr.v1().z(),
				fr.v2().x(), fr.v2().y(), fr.v2().z());
		}
		for (int phi = 0; phi < _phisteps; phi++) {
			double	_phi = phi * deltaphi;
			add_vertex(B, w + _r *
				(cos(_phi) * fr.v3() + sin(_phi) * fr.v2()));
		}
	}

	add_vertex(B, _f.position(_interval.max()));

	// add all facets
	
	// start cap
	for (int phi = 0; phi < _phisteps - 1; phi++) {
		add_facet(B, 0, vertex(0, phi), vertex(0, phi + 1));
	}
	add_facet(B, 0, _phisteps , 1);

	// intermediate zones
	for (int t = 0; t < _steps; t++) {
		if (debug) {
			fprintf(stderr, "%s:%d: facets for t = %d\n",
				__FILE__, __LINE__, t);
		}
		for (int phi = 0; phi < _phisteps - 1; phi++) {
			add_facet(B,
				vertex(t    , phi    ),
				vertex(t + 1, phi    ),
				vertex(t    , phi + 1));
			add_facet(B,
				vertex(t    , phi + 1),
				vertex(t + 1, phi    ),
				vertex(t + 1, phi + 1));
		}
		add_facet(B,
			vertex(t    , _phisteps - 1    ),
			vertex(t + 1, _phisteps - 1    ),
			vertex(t    , 0));
		add_facet(B,
			vertex(t    , 0),
			vertex(t + 1, _phisteps - 1    ),
			vertex(t + 1, 0));
	}

	// end cap
	if (debug) {
		fprintf(stderr, "%s:%d: end cap\n", __FILE__, __LINE__);
	}
	for (int phi = 0; phi < _phisteps - 1; phi++) {
		add_facet(B,
			vertex(_steps, phi),
			vertexnumber() - 1,
			vertex(_steps, phi + 1));
	}
	add_facet(B,
		vertex(_steps, _phisteps - 1),
		vertexnumber() - 1,
		vertex(_steps, 0));

	// that's it, we are done
	B.end_surface();
}

} // namespace csg
