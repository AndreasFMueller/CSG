/*
 * Line.cpp -- straight line visualization
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <Line.h>

namespace csg {

vector	Line::normal(double /* t */) const {
	if (vector::e1.parallel(_r)) {
		return vector::e2.orthogonalto(_r);
	}
	return vector::e1.orthogonalto(_r);
}

} // namespace csg
