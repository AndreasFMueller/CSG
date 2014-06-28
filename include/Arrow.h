/*
 * Arrow.h -- solid arrow primitive
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _Arrow_h
#define _Arrow_H

#include <common.h>
#include <Surface.h>

namespace csg {

class Build_Arrow : public Build_Surface {
	point	_from;
	point	_to;
	double	_radius;
	int _steps;
	int	vertex(int phi, int z) const;
public:
	Build_Arrow(const point& from, const point& to, double radius,
		int steps = 4)
		: _from(from), _to(to), _radius(radius), _steps(steps) {
	}
	void	operator()(Polyhedron::HalfedgeDS& hds);
};

} // namespace csg

#endif /* _Arrow_h */
