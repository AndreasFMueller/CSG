/*
 * solution.h -- solution functions for pde1
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _solution_h
#define _solution_h

#include <common.h>
#include <Cartesian.h>

namespace csg {

extern bool yzslicing;

class Solution : public PointFunction {
public:
	virtual point	p(double y0, double t) const;
	virtual vector	v(double y0, double t) const;
};

class BaseSolution : public PointFunction {
public:
	virtual point	p(double x, double y) const;
	virtual vector	v(double x, double y) const;
};

Nef_polyhedron	build_solution(double thickness);

class AlternativeSolution : public PointFunction {
	double	mu;
	double	F(double x, double y) const;
	static double	gamma;
	double	x(double xi, double eta) const;
	double	y(double xi, double eta) const;
public:
	AlternativeSolution(double _mu);
	virtual point	p(double xi, double eta) const;
	virtual vector	v(double xi, double eta) const;
};

Nef_polyhedron   build_alternative(double thickness);

} // namespace csg

#endif /* _solution_h */
