/*
 * Parts.h -- auxiliary classes to cat objects using half spaces
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _Parts_h
#define _Parts_h

#include <common.h>
#include <string>

namespace csg {

class PartWriter {
	std::string	prefix;
	void	write_part(Nef_polyhedron& image, const std::string& part,
			const Nef_polyhedron& halfspace) const;
public:
	PartWriter(const std::string& prefix);
	typedef enum parts {
		LEFT_PART, RIGHT_PART,
		FRONT_PART, BACK_PART,
		TOP_PART, BOTTOM_PART
	} object_part;
	void	operator()(const object_part& part, Nef_polyhedron& image) const;
};

} // namespace csg

#endif /* _Parts_h */
