/*
 * hyperbola.cpp -- hyperbola to construct supports
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <hyperbola.h>
#include <math.h>
#include <debug.h>

namespace csg {

hyperbola::hyperbola(const point2& p1, const point2& p2, double c) {
	double  sign = (c > 0) ? 1. : -1.;
	double  deltax = p2.x - p1.x;
	double  deltay = p2.y - p1.y;
	c2 = c * c * ((deltax * deltay > 0) ? -1 : 1);
	double  k1 = p1.x * p1.y - c2;
	double  k2 = p2.x * p2.y - c2;
	double  deltak = k2 - k1;
	debug(LOG_DEBUG, DEBUG_LOG, 0,
		"deltax = %f, deltay = %f, deltak = %f",
		deltax, deltay, deltak);
	double  A = deltay / deltax;
	double  B = p1.y - p1.x * deltay/deltax - deltak/deltax;
	double  C = p1.x * deltak / deltax - k1;
	debug(LOG_DEBUG, DEBUG_LOG, 0, "A = %f, B = %f, C = %f",
		A, B, C);
	a = (-B + sign * sqrt(B * B - 4 * A * C)) / (2 * A);
	b = (deltak - a * deltay) / deltax;
	debug(LOG_DEBUG, DEBUG_LOG, 0, "a = %f, b = %f, c2 = %f",
		a, b, c2);
}

double	hyperbola::operator()(const double x) const {
	double  y = b + c2 / (x - a);
//	debug(LOG_DEBUG, DEBUG_LOG, 0, "x = %f -> y = %f", x, y);
	return y;
}

} // namespace csg
