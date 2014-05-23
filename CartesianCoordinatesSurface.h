/*
 * CartesianCoordinatesSurface.h -- builder class for 
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _CartesianCoordinatesSurface_h
#define _CartesianCoordinatesSurface_h

#include <common.h>
#include <Surface.h>

namespace csg {

/**
 * \brief Build surfaces parametrized by cartesian coordinate 
 */
class Build_Cartesian : public Build_Surface {
	CartesianDomain	_domain;
protected:
	virtual point	p(double x, double y, double h) = 0;
private:
	int	_xsteps, _ysteps;
	double	_h;
	int	vertex(const int x, const int y) const;
public:
	Build_Cartesian(const CartesianDomain& domain,
		int xsteps, int ysteps, double h)
		: _domain(domain), _xsteps(xsteps), _ysteps(ysteps), _h(h) {
	}
	void	operator()(Polyhedron::HalfedgeDS& hds);
};

/**
 * \brief Create a surface based on a z-function
 */
class Build_CartesianFunction : public Build_Cartesian {
	Function&	_f;
protected:
	virtual point	p(double x, double y, double h);
public:
	Build_CartesianFunction(Function& f, const CartesianDomain& domain,
		int xsteps, int ysteps, double h)
		: Build_Cartesian(domain, xsteps, ysteps, h), _f(f) {
	}
};

/**
 * \brief Create a surface basd on vector function with cartesian coordinates
 */
class Build_CartesianPointFunction : public Build_Cartesian {
	PointFunction&	_f;
protected:
	virtual point	p(double x, double y, double h);
public:
	Build_CartesianPointFunction(PointFunction& f,
		const CartesianDomain& domain, int xsteps, int ysteps, double h)
		: Build_Cartesian(domain, xsteps, ysteps, h), _f(f) {
	}
};

} // namespace csg

#endif /* _CartesianCoordinatesSurface_h */
