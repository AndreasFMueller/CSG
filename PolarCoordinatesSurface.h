/*
 * PolarCoordinatesSurface.h -- builder class for 
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _PolarCoordinatesSurface_h
#define _PolarCoordinatesSurface_h

#include <common.h>

namespace csg {

class Build_PolarCoordinatesSurface
	: public CGAL::Modifier_base<Polyhedron::HalfedgeDS> {
	PolarFunction&	_f;
	int	_rsteps, _phisteps;
	double	_h;
	// members used during constrution
	double	deltar, deltaphi;
	double	_phi, _r;
	int	vertexnumber;
	int	facenumber;
	// add vertices
	void	add_vertices(CGAL::Polyhedron_incremental_builder_3<Polyhedron::HalfedgeDS>& B);
	void	add_surface_triangles(CGAL::Polyhedron_incremental_builder_3<Polyhedron::HalfedgeDS>& B);

	// methods used during creation
	int	open_vertex(const int x, const int y) const;
	int	open0_vertex(const int x, const int y) const;
	int	closed_vertex(const int x, const int y) const;
	int	closed0_vertex(const int x, const int y) const;
	void	open_surface(Polyhedron::HalfedgeDS& hds);
	void	open0_surface(Polyhedron::HalfedgeDS& hds);
	void	closed_surface(Polyhedron::HalfedgeDS& hds);
	void	closed0_surface(Polyhedron::HalfedgeDS& hds);
public:
	Build_PolarCoordinatesSurface(PolarFunction& f, int rsteps,
		int phisteps, double h)
		: _f(f), _rsteps(rsteps), _phisteps(phisteps), _h(h) {
		deltar = _f.rrange().length() / _rsteps;
		deltaphi = _f.phirange().length() / _phisteps;
		vertexnumber = 0;
	}
	void	operator()(Polyhedron::HalfedgeDS& hds);
};

} // namespace csg

#endif /* _PolarCoordinatesSurface_h */
