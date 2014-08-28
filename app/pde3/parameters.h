/*
 * parameters.h -- parameters for the PDE without solution
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _parameters_h
#define _parameters_h

namespace csg {

extern double	thickness;
extern double	h;
extern int	steps;
extern double	arrowdiameter;
extern double	smallcurveradius;
extern double	a;
extern double	f(double x);

};

#endif /* _parameters_h */
