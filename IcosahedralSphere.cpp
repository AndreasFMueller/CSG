/*
 * IcosahedralSphere.cpp -- Create a spherical mesh based 
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <IcosahedralSphere.h>

#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <stdexcept>

namespace csg {

void	Build_IcosahedralSphere::operator()(Polyhedron::HalfedgeDS& hds) {
	CGAL::Polyhedron_incremental_builder_3<Polyhedron::HalfedgeDS>	B(hds, true);

	throw std::logic_error("IcosahedralSphere not implemented yet");
	B.begin_surface(0, 0, 0);

	B.end_surface();
}

} // namespace csg
