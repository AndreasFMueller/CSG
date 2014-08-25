/*
 * solution.h -- solution function for pde3
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _solution_h
#define _solution_h

#include <common.h>

namespace csg {

class Solution : public PointFunction {
	double	a;
public:
	Solution(double _a) : a(_a) { }
	virtual point	p(double r0, double t) const;
	virtual vector	v(double r0, double t) const;
};

Nef_polyhedron	build_solution(double thickness);

} // namespace csg

#endif /* _solution_h */
