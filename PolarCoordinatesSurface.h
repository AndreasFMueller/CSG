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

/**
 * \brief build a surface on a polar domain
 */
class Build_Polar : public Build_Surface {
	PolarDomain	_domain;
protected:
	virtual point	p(double r, double phi, double h) = 0;
private:
	// grid parametrization
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
	Build_Polar(const PolarDomain& domain,
		int rsteps, int phisteps, double h)
		: _domain(domain), _rsteps(rsteps), _phisteps(phisteps), _h(h) {
		deltar = _domain.rrange().length() / _rsteps;
		deltaphi = _domain.phirange().length() / _phisteps;
	}
	void	operator()(Polyhedron::HalfedgeDS& hds);
};

/**
 * \brief Build a polar coordinate parametrized surface, where only z is given
 */
class Build_PolarFunction : public Build_Polar {
	Function&	_f;
protected:
	virtual point	p(double r, double phi, double h);
public:
	Build_PolarFunction(Function& f, const PolarDomain& domain,
		int rsteps, int phisteps, double h)
		: Build_Polar(domain, rsteps, phisteps, h), _f(f) {
	}
};

/**
 * \brief Build  polar coordinate parametrized surface from a vector function
 */
class Build_PolarPointFunction : public Build_Polar {
	PointFunction&	_f;
protected:
	virtual point	p(double r, double phi, double h);
public:
	Build_PolarPointFunction(PointFunction& f, const PolarDomain& domain,
		int rsteps, int phisteps, double h)
		: Build_Polar(domain, rsteps, phisteps, h), _f(f) {
	}
};

} // namespace csg

#endif /* _PolarCoordinatesSurface_h */
