/*
 * IcosahedralSphere.h
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _IcosahedralSphere_h
#define _IcosahedralSphere_h

#include <common.h>

namespace csg {

class Build_IcosahedralSphere : public CGAL::Modifier_base<Polyhedron::HalfedgeDS> {
	double	_radius;
	int	_steps;
public:
	Build_IcosahedralSphere(double radius, int steps)
		: _radius(radius), _steps(steps) {
	}
	void	operator()(Polyhedron::HalfedgeDS& hds);
};

} // namespace csg

#endif /* _IcosahedralSphere_h */
