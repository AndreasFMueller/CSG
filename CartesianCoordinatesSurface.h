/*
 * CartesianCoordinatesSurface.h -- builder class for 
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _CartesianCoordinatesSurface_h
#define _CartesianCoordinatesSurface_h

#include <common.h>

namespace csg {

class Build_CartesianCoordinatesSurface : public CGAL::Modifier_base<Polyhedron::HalfedgeDS> {
	CartesianFunction&	_f;
	int	_xsteps, _ysteps;
	double	_h;
	int	vertex(const int x, const int y) const;
public:
	Build_CartesianCoordinatesSurface(CartesianFunction& f, int xsteps, int ysteps, double h)
		: _f(f), _xsteps(xsteps), _ysteps(ysteps), _h(h) {
	}
	void	operator()(Polyhedron::HalfedgeDS& hds);
};

} // namespace csg

#endif /* _CartesianCoordinatesSurface_h */
