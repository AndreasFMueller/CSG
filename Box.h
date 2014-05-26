/*
 * Box.h -- create a box
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _Box_h
#define _Box_h

#include <common.h>
#include <Surface.h>

namespace csg {

class Build_Box : public Build_Surface {
	point	_a, _b;
	void	standardize();
public:
	Build_Box(const point& a, const point& b) : _a(a), _b(b) {
		standardize();
	}
	void	operator()(Polyhedron::HalfedgeDS& hds);
};

} // namespace csg

#endif /* _Box_h */
