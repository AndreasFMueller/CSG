/*
 * solution.h -- build the solution 
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */

#include <solution.h>
#include <parameters.h>
#include <debug.h>
#include <Cartesian.h>

namespace csg {

/**
 *  \brief Function class representing the solution of the PDE
 */
class Solution : public Function {
public:
	Solution() { }
	virtual double	operator()(const double x, const double y) {
		return exp(-x) * sin(y - x * x / 2);
	}
};

Nef_polyhedron	build_solution() {
	debug(LOG_DEBUG, DEBUG_LOG, 0, "adding solution surface");
	CartesianDomain	domain(Interval(0, 1.8), Interval(0, M_PI));
	Solution	sol;
	Build_CartesianFunction	b(sol, domain, curvesteps, curvesteps, 0.03);
	Polyhedron	p;
	p.delegate(b);
	return Nef_polyhedron(p);
}

} // namespace csg
