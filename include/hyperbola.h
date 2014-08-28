/*
 * hyperbola.h -- functor class to construct hyperbolas
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _hyperbola_h
#define _hyperbola_h

namespace csg {

class point2 {
public:
	double x;
	double y;
	point2(double _x, double _y) : x(_x), y(_y) { }
};

class hyperbola {
        double  a, b, c2;
public:
        hyperbola(const point2& p1, const point2& p2, double c = 1);
        double  operator()(const double x) const;
};

} // namespace csg

#endif /* _hyperbola_h */
