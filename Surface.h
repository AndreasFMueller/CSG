/*
 * Surface.h -- base class for the Build_Surface classes
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _Surface_h
#define _Surface_h

#include <common.h>

namespace csg {

class Build_Surface : public CGAL::Modifier_base<Polyhedron::HalfedgeDS> {
	int	_vertexnumber;
public:
	const int&	vertexnumber() const { return _vertexnumber; }
private:
	int	_facetnumber;
public:
	const int&	facetnumber() const { return _facetnumber; }
public:
	Build_Surface() {
		_vertexnumber = 0;
		_facetnumber = 0;
	}
protected:
	void	add_vertex(Builder& B, double x, double y, double z);
	void	add_facet(Builder& B, int a, int b, int c);
};

} // namespace csg

#endif /* _Surface_h */
