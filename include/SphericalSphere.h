/*
 * SphericalSphere.h -- Create a spherical mesh based 
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _SphericalSphere_h
#define _SphericalSphere_h

#include <common.h>
#include <Surface.h>

namespace csg {

class Build_SphericalSphere : public Build_Surface {
	double	_radius;
	int	_steps;
public:
	Build_SphericalSphere(double radius, int steps)
		: _radius(radius), _steps(steps) {
	}
	void	operator()(Polyhedron::HalfedgeDS& hds);
};

} // namespace csg

#endif /* _SphericalSphere_h */
