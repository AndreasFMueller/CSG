/*
 * Surface.cpp -- common methods for building surfaces
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <Surface.h>
#include <debug.h>

namespace csg {

void	Build_Surface::add_vertex(Builder& B, double x, double y, double z) {
	if (debuglevel > LOG_DEBUG) {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "vertex %d (%f, %f, %f)",
			vertexnumber(), x, y, z);
	}
	B.add_vertex(Point(x, y, z));
	_vertexnumber++;
}

void	Build_Surface::add_vertex(Builder& B, const point& p) {
	add_vertex(B, p.x(), p.y(), p.z());
}

void	Build_Surface::add_facet(Builder& B, int a, int b, int c) {
	if (debuglevel > LOG_DEBUG) {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "facet  %d (%d, %d, %d)",
			facetnumber(), a, b, c);
	}
	if (a >= vertexnumber()) {
		fprintf(stderr, "vertex number %d exceeds %d\n", a,
			vertexnumber());
		throw std::runtime_error("bad vertex number");
	}
	if (b >= vertexnumber()) {
		fprintf(stderr, "vertex number %d exceeds %d\n", b,
			vertexnumber());
		throw std::runtime_error("bad vertex number");
	}
	if (c >= vertexnumber()) {
		fprintf(stderr, "vertex number %d exceeds %d\n", c,
			vertexnumber());
		throw std::runtime_error("bad vertex number");
	}
	B.begin_facet();
	B.add_vertex_to_facet(a);
	B.add_vertex_to_facet(b);
	B.add_vertex_to_facet(c);
	B.end_facet();
	_facetnumber++;
}

} // namespace csg
