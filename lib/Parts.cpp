/*
 * Parts.cpp
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <Parts.h>
#include <debug.h>
#include <fstream>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>

namespace csg {

PartWriter::PartWriter(const std::string& _prefix) : prefix(_prefix) {
}

void	PartWriter::write_part(Nef_polyhedron& image, const std::string& part,
		const Nef_polyhedron& halfspace) const {
	try {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "writing part: %s",
			part.c_str());
		std::string	name = prefix + std::string("-") + part
					+ std::string(".off");
		std::ofstream	out(name.c_str());
		Polyhedron	P;
		(halfspace * image).convert_to_polyhedron(P);
		debug(LOG_DEBUG, DEBUG_LOG, 0,
			"conversion to polygon complete");
		out << P;
		out.close();
		debug(LOG_DEBUG, DEBUG_LOG, 0, "part %s written",
			part.c_str());
	} catch (std::exception& x) {
		fprintf(stderr, "error while writing back part: %s\n",
			x.what());
	} catch (...) {
		fprintf(stderr, "error while writing back part\n");
	}
}

void	PartWriter::operator()(const object_part& part,
			Nef_polyhedron& image) const {
	switch (part) {
	case LEFT_PART:
		write_part(image, std::string("left"),
			Nef_polyhedron(Plane(1, 0, 0, 0),
				Nef_polyhedron::INCLUDED));
		break;
	case RIGHT_PART:
		write_part(image, std::string("right"), 
			Nef_polyhedron(Plane(-1, 0, 0, 0),
				Nef_polyhedron::INCLUDED));
		break;
	case FRONT_PART:
		write_part(image, std::string("front"),
			Nef_polyhedron(Plane(0, 1, 0, 0),
				Nef_polyhedron::INCLUDED));
		break;
	case BACK_PART:
		write_part(image, std::string("back"),
			Nef_polyhedron(Plane(0, -1, 0, 0),
				Nef_polyhedron::INCLUDED));
		break;
	case TOP_PART:
		write_part(image, std::string("top"),
			Nef_polyhedron(Plane(0, 0, 1, 0),
				Nef_polyhedron::INCLUDED));
		break;
	case BOTTOM_PART:
		write_part(image, std::string("bottom"),
			Nef_polyhedron(Plane(0, 0, -1, 0),
				Nef_polyhedron::INCLUDED));
		break;
	}
}

} // namespace csg
