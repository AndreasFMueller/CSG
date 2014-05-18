/*
 * Arrow.cpp -- create an arrow
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <Arrow.h>

namespace csg {

void	Build_Arrow::operator()(Polyhedron::HalfedgeDS& hds) {
	CGAL::Polyhedron_incremental_builder_3<Polyhedron::HalfedgeDS>	B(hds, true);
	B.begin_surface(0, 0, 0);
	B.end_surface();
}

} // namespace csg
