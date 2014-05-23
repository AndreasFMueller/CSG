/*
 * SphericalSurface.h -- Create a spherical mesh based 
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _SphericalSurface_h
#define _SphericalSurface_h

#include <common.h>
#include <Surface.h>

namespace csg {

class SphericalFunction {
public:
	virtual double	operator()(const double theta, const double phi) = 0;
};

class Build_SphericalSurface : public Build_Surface {
	SphericalFunction&	_f;
	int	_steps;
public:
	Build_SphericalSurface(SphericalFunction& f, int steps)
		: _f(f), _steps(steps) {
	}
	void	operator()(Polyhedron::HalfedgeDS& hds);
};

} // namespace csg

#endif /* _SphericalSurface_h */
