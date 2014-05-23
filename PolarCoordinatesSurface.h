/*
 * PolarCoordinatesSurface.h -- builder class for 
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _PolarCoordinatesSurface_h
#define _PolarCoordinatesSurface_h

#include <common.h>
#include <Surface.h>

namespace csg {

class Build_PolarCoordinatesSurface : public Build_Surface {
	PolarFunction&	_f;
	int	_rsteps, _phisteps;
	double	_h;
	// members used during constrution
	double	deltar, deltaphi;
	double	_phi, _r;
	// add vertices
	void	add_vertices(Builder& B);
	void	add_surface_triangles(Builder& B);
	void	add_surface_fan(Builder& B);
	void	add_radius_surface(Builder& B);
	void	add_perimeter(Builder& B);

	// information about the structure
	bool	closed() const;
	bool	contains0() const;

	// methods used during creation
	int	vertex(const int x, const int y) const;
public:
	Build_PolarCoordinatesSurface(PolarFunction& f, int rsteps,
		int phisteps, double h)
		: _f(f), _rsteps(rsteps), _phisteps(phisteps), _h(h) {
		deltar = _f.rrange().length() / _rsteps;
		deltaphi = _f.phirange().length() / _phisteps;
	}
	void	operator()(Polyhedron::HalfedgeDS& hds);
};

} // namespace csg

#endif /* _PolarCoordinatesSurface_h */
