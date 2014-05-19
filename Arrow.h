/*
 * Arrow.h -- solid arrow primitive
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _Arrow_h
#define _Arrow_H

#include <common.h>

namespace csg {

class Build_Arrow : public CGAL::Modifier_base<Polyhedron::HalfedgeDS> {
	Point	_from;
	Point	_to;
	double	_radius;
	int _steps;
public:
	Build_Arrow(const Point& from, const Point& to, double radius,
		int steps = 4)
		: _from(from), _to(to), _radius(radius), _steps(steps) {
	}
	void	operator()(Polyhedron::HalfedgeDS& hds);
};

} // namespace csg

#endif /* _Arrow_h */
