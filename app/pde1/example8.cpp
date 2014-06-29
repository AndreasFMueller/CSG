/*
 * example8.cpp -- example 8, nonuniqueness of the solution of a partial
 *                            differential equation
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <common.h>
#include <debug.h>
#include <Box.h>
#include <SphericalSphere.h>
#include <Cartesian.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <math.h>
#include <Curve.h>
#include <Arrow.h>
#include <CGAL/Nef_nary_union_3.h>
#include <fstream>

namespace csg {

typedef CGAL::Nef_nary_union_3<Nef_polyhedron>  Nef_nary_union;

bool	yzslicing = true;

std::string	prefix("hypchar");
const double	a = 0.25;
const double	speed = 4;
int	steps = 30;
const	double	charstep = 0.25;
int	phisteps = 12;

double	h0(double x) {
	return exp(-log(2) * 0.5 * 0.5 / (x * x));
}

double	h(double x) {
	if (x <= 0) {
		return 0;
	}
	if (x <= 0.5) {
		return h0(x);
	}
	if (x < 1) {
		return 1 - h0(1 - x);
	}
	return 1;
}

double	f(double x, double mu) {
	return h(x / speed) * mu * x - (1 - h(x / speed)) * a * x * x;
}

class Solution : public PointFunction {
public:
	virtual point	p(double y0, double t) const {
		return point(y0 * sinh(t / y0), y0 * cosh(t / y0), a * y0 * y0);
	}
	virtual vector	v(double y0, double t) const {
		double	s = sinh(t / y0);
		double	c = cosh(t / y0);
		double	x = -2 * a * y0 * s;
		double	y = 2 * a * y0 * c;
		double	z = s * (s - (t / y0) * c) - c * (c - (t / y0) * s);
		return vector(x, y, z).normalized();
	}
};

class BaseSolution : public PointFunction {
public:
	virtual point	p(double x, double y) const {
		return point(x, y, a * (y * y - x * x));
	}
	virtual vector	v(double x, double y) const {
		if (yzslicing) {
			double	derivative = 2 * a * y;
			return vector(0., -derivative, 1.).normalized();
		} else {
			double	derivative = -2 * a * x;
			return vector(-derivative, 0., 1.).normalized();
		}
	}
};

class AlternativeSolution : public PointFunction {
	double	mu;
	double	F(double x, double y) const {
		if (fabs(x) <= fabs(y)) {
			return 0;
		}
		return f(sqrt(x * x - y * y), mu);
	}
	static double	gamma;
	double	x(double xi, double eta) const {
		return 4 * (pow(eta, gamma) + pow(xi, gamma));
	}
	double	y(double xi, double eta) const {
		return 4 * (pow(eta, gamma) - pow(xi, gamma));
	}
public:
	AlternativeSolution(double _mu) : mu(_mu) { }
	virtual point	p(double xi, double eta) const {
		double	xx = x(xi, eta);
		double	yy = y(xi, eta);
		return point(xx, yy, F(xx, yy));
	}
	virtual vector	v(double xi, double eta) const {
		double	xx = x(xi, eta);
		double	yy = y(xi, eta);
		if (yzslicing) {
			double	dy = 0.0001;
			double	derivative = (F(xx, yy + dy) - F(xx, yy)) / dy;
			vector	n =  vector(0., -derivative, 1.).normalized();
			return n;
		} else {
			double	dx = 0.0001;
			double	derivative = (F(xx + dx, yy) - F(xx, yy)) / dx;
			vector	n =  vector(-derivative, 0., 1.).normalized();
			return n;
		}
	}
};
double	AlternativeSolution::gamma = 1.5;

class SupportSheet : public PointFunction {
public:
	virtual point	p(double x, double z) const {
		return point(x, 0, -z - a * x * x);
	}
	virtual vector	v(double x, double z) const {
		return vector::e2;
	}
};

class CharacteristicY : public CurveFunction {
	double	y0;
public:
	CharacteristicY(const double _y0, double deltat = 0)
		: CurveFunction(deltat), y0(_y0) { }
	virtual point	position(double t) const {
		point	p(y0 * sinh(t / y0), y0 * cosh(t / y0), a * y0 * y0);
		return p;
	}
	virtual vector	tangent(double t) const {
		vector	v(cosh(t / y0), sinh(t / y0), 0);
		return v;
	}
	virtual vector	normal(double t) const {
		vector	n(sinh(t / y0) / y0, cosh(t / y0) / y0, 0);
		return n;
	}
};

class CharacteristicX : public CurveFunction {
	double	x0;
	double	z;
public:
	CharacteristicX(const double _x0, double _z, double deltat = 0)
		: CurveFunction(deltat), x0(_x0), z(_z) { }
	virtual point	position(double t) const {
		point	p(x0 * cosh(t / x0), x0 * sinh(t / x0), z);
		return p;
	}
	virtual vector	tangent(double t) const {
		vector	v(sinh(t / x0), cosh(t / x0), 0);
		return v;
	}
	virtual	vector	normal(double t) const {
		vector	n(cosh(t / x0) / x0, sinh(t / x0) / x0, 0);
		return n;
	}
};

class Asymptote : public CurveFunction {
	double	m;
public:
	Asymptote(const double _m, double deltat = 0)
		: CurveFunction(deltat), m(_m) { }
	virtual point	position(double t) const {
		return point(t, m * t, 0.);
	}
	virtual vector	tangent(double t) const {
		return vector(1., m, 0.);
	}
	virtual vector	normal(double t) const {
		return vector::e2;
	}
};

class InitialCurve : public CurveFunction {
public:
	virtual point	position(double t) const {
		return point(0., t, a * t * t);
	}
	virtual vector	tangent(double t) const {
		return vector(0., 1., 2 * a * t);
	}
	virtual vector	normal(double t) const {
		return vector(0., 0., 2 * a);
	}
};

class FCurve : public CurveFunction {
public:
	virtual point	position(double t) const {
		return point(t, 0., -a * t * t);
	}
	virtual vector	tangent(double t) const {
		return vector(1., 0., -2 * a * t);
	}
	virtual vector	normal(double t) const {
		return vector(0., 0., -2 * a);
	}
};

double	sheetthickness = 0.05;
double	smallcurveradius = 0.015;
double	largecurveradius = 0.040;

static Nef_polyhedron	build_alternative(double thickness) {
	debug(LOG_DEBUG, DEBUG_LOG, 0, "build alternative solution surfaces");
	Nef_nary_union	unioner;
	CartesianDomain	xietadomain(Interval(0, 1), Interval(0, 1));
	for (double m = 0.5; m > -0.6; m -= 0.25) {
	//for (double m = 0.25; m > 0.2; m -= 0.25) {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "m = %f", m);
		AlternativeSolution	altsolution(m);
		Build_CartesianPointFunction	a(altsolution,
			xietadomain, 2 * steps, 2 * steps, thickness);
		Polyhedron	p;
		p.delegate(a);
		Nef_polyhedron	n(p);
		unioner.add_polyhedron(n);
	}
	debug(LOG_DEBUG, DEBUG_LOG, 0, "get union");
	return unioner.get_union();
}

static void	add_characteristic(Nef_nary_union& unioner, double y0) {
	debug(LOG_DEBUG, DEBUG_LOG, 0, "add characteristic at y0 = %f", y0);
	double	tmax = fabs(y0) * acosh(2 / fabs(y0));
	debug(LOG_DEBUG, DEBUG_LOG, 0, "t interval extends to %f", tmax);
	Interval	interval(0, tmax + 0.2);
	CharacteristicY	cs(y0);
	Build_Curve	charcurve(cs, interval, steps, 8,
				smallcurveradius);
	Polyhedron	p;
	p.delegate(charcurve);
	Nef_polyhedron	n(p);
	unioner.add_polyhedron(n);
	debug(LOG_DEBUG, DEBUG_LOG, 0, "curve added");
}

static void	add_asymptote(Nef_nary_union& unioner, double m) {
	debug(LOG_DEBUG, DEBUG_LOG, 0, "adding asymptote %f", m);
	Interval	interval(0., 2.1);
	Asymptote	asymptote(m);
	Build_Curve	charcurve(asymptote, interval, steps, phisteps,
				smallcurveradius);
	Polyhedron	p;
	p.delegate(charcurve);
	Nef_polyhedron	n(p);
	unioner.add_polyhedron(n);
	debug(LOG_DEBUG, DEBUG_LOG, 0, "asymptote %f added", m);
}

static Nef_polyhedron	build_xcharacteristics() {
	Nef_nary_union	unioner;
	for (double x0 = charstep; a * x0 * x0 < 2; x0 += charstep) {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "characteristic for x0 = %f",
			x0);
		Polyhedron	p;
		double	tmax = x0 * asinh(2 / x0);
		Interval	interval(-tmax - 0.2, tmax + 0.2);
		CharacteristicX	cx(x0, -a * x0 * x0);
		Build_Curve	charcurve(cx, interval, steps, phisteps,
					smallcurveradius);
		p.delegate(charcurve);
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add characteristic for x0 = %f",
			x0);
		unioner.add_polyhedron(p);
	}
	return unioner.get_union();
}

static Nef_polyhedron	build_characteristics() {
	Nef_nary_union	unioner;
	debug(LOG_DEBUG, DEBUG_LOG, 0, "add 1st quadrant characteristics");
	for (double y0 = charstep * floor(2 / charstep); y0 > 0.1;
			y0 -= charstep) {
		add_characteristic(unioner, y0);
	}
	debug(LOG_DEBUG, DEBUG_LOG, 0, "add asymptotes");
	add_asymptote(unioner, 1);
	add_asymptote(unioner, -1);
	debug(LOG_DEBUG, DEBUG_LOG, 0, "add 4th quadrant characteristics");
	for (double y0 = -charstep; y0 > -2.1; y0 -= charstep) {
		add_characteristic(unioner, y0);
	}
	debug(LOG_DEBUG, DEBUG_LOG, 0, "extract union of characteristics");
	return unioner.get_union();
}

static Nef_polyhedron	build_axes() {
	Nef_nary_union	unioner;
	{
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add X-axis");
		Polyhedron	p;
		Build_Arrow	b(point(-0.1, 0, 0),
					point(4.1, 0, 0), smallcurveradius, 16);
		p.delegate(b);
		unioner.add_polyhedron(p);
	}
	{
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add Y-axis");
		Polyhedron	p;
		Build_Arrow	b(point(0, -2, 0),
					point(0, 2, 0), smallcurveradius, 16);
		p.delegate(b);
		unioner.add_polyhedron(p);
	}
	{
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add Z-axis");
		Polyhedron	p;
		Build_Arrow	b(point(0, 0, -2),
					point(0, 0, 2), smallcurveradius, 16);
		p.delegate(b);
		unioner.add_polyhedron(p);
	}
	debug(LOG_DEBUG, DEBUG_LOG, 0, "extract axes union");
	return unioner.get_union();
}

static Nef_polyhedron	build_support() {
	CartesianDomain	domain(Interval(0, 4), Interval(0, 2));
	SupportSheet	support;
	Build_CartesianPointFunction	s(support,
			domain, steps, steps, 0.5 * sheetthickness);
	Polyhedron	p;
	p.delegate(s);
	return	Nef_polyhedron(p);
}

static Nef_polyhedron	build_fcurve() {
	Polyhedron	p;
	Interval	interval(0, 3);
	FCurve	fc;
	Build_Curve	fcurve(fc, interval,
				steps, phisteps, largecurveradius);
	p.delegate(fcurve);
	return Nef_polyhedron(p);
}

static void	write_part(Nef_polyhedron& image, const std::string& part,
		const Nef_polyhedron& halfspace) {
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

bool	show_solution = true;
bool	show_characteristics = true;
bool	show_alternatives = true;
bool	show_initial = true;
bool	show_axes = true;
bool	show_negative = true;
bool	show_support = false;
bool	show_fcurve = true;

/** 
 * \brief main function for example6
 */
int	main(int argc, char *argv[]) {

	int	c;
	while (EOF != (c = getopt(argc, argv, "dSACIXNPFx")))
		switch (c) {
		case 'd':
			if (debuglevel == LOG_DEBUG) {
				debuglevel++;
			} else {
				debuglevel = LOG_DEBUG;
			}
			break;
		case 'S':
			show_solution = !show_solution;
			break;
		case 'A':
			show_alternatives = !show_alternatives;
			break;
		case 'C':
			show_characteristics = !show_characteristics;
			break;
		case 'I':
			show_initial = !show_initial;
			break;
		case 'X':
			show_axes = !show_axes;
			break;
		case 'N':
			show_negative = !show_negative;
			break;
		case 'F':
			show_fcurve = !show_fcurve;
			break;
		case 'P':
			show_support = !show_support;
			break;
		case 'p':
			prefix = std::string(optarg);
			break;
		case 'x':
			yzslicing = false;
			break;
		}

	debug(LOG_DEBUG, DEBUG_LOG, 0, "nonuniqueness of solution of a "
		"partial differential equation");

	// start building up the union of things to be restricted to a box
	Nef_nary_union	unioner;

	// create a the solution surface
	try {
		if (show_solution) {
			debug(LOG_DEBUG, DEBUG_LOG, 0, "adding solution surface");
			Polyhedron	p;
			CartesianDomain	domain(Interval(0, 4), Interval(-2, 2));
			BaseSolution	basesolution;
			Build_CartesianPointFunction	b(basesolution, domain,
				2 * steps, 2 * steps, 0.5 * sheetthickness);
			p.delegate(b);
			Nef_polyhedron	n(p);
			unioner.add_polyhedron(n);
			debug(LOG_DEBUG, DEBUG_LOG, 0, "solution surface added");
		}
	} catch (std::exception& x) {
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add solution: %s",
			x.what());
	}

	// create an alternative solution surface
	try {
		if (show_alternatives) {
			debug(LOG_DEBUG, DEBUG_LOG, 0, "adding alternatives");
			unioner.add_polyhedron(build_alternative(0.5 * sheetthickness));
			debug(LOG_DEBUG, DEBUG_LOG, 0, "alternatives added");
		}
	} catch (std::exception& x) {
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add alternatives: %s",
			x.what());
	}

	// add additional characteristics
	try {
		if (show_characteristics) {
			debug(LOG_DEBUG, DEBUG_LOG, 0, "adding characteristics");
			unioner.add_polyhedron(build_characteristics());
			debug(LOG_DEBUG, DEBUG_LOG, 0, "characteristics added");
		}
	} catch (std::exception& x) {
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add characteristics: %s",
			x.what());
	}

	// add negative characteristics
	try {
		if (show_negative) {
			debug(LOG_DEBUG, DEBUG_LOG, 0, "adding neg characteristics");
			unioner.add_polyhedron(build_xcharacteristics());
			debug(LOG_DEBUG, DEBUG_LOG, 0, "neg characteristics added");
		}
	} catch (std::exception& x) {
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add negative characteristics: %s",
			x.what());
	}

	// add the FCurve
	try {
		if (show_fcurve) {
			debug(LOG_DEBUG, DEBUG_LOG, 0, "adding fcurve");
			unioner.add_polyhedron(build_fcurve());
			debug(LOG_DEBUG, DEBUG_LOG, 0, "fcurve added");
		}
	} catch (std::exception& x) {
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add fcurve: %s",
			x.what());
	}

	// add the support sheet
	try {
		if (show_support) {
			debug(LOG_DEBUG, DEBUG_LOG, 0, "adding support");
			unioner.add_polyhedron(build_support());
			debug(LOG_DEBUG, DEBUG_LOG, 0, "support added");
		}
	} catch (std::exception& x) {
		debug(LOG_ERR, DEBUG_LOG, 0, "failed to add support: %s",
			x.what());
	}

	// add the initial curve
	if (show_initial) {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "adding initial curve");
		Polyhedron	p;
		Interval	interval(-2.1, 2.1);
		InitialCurve	ic;
		Build_Curve	initialcurve(ic, interval,
					steps, phisteps, largecurveradius);
		p.delegate(initialcurve);
		unioner.add_polyhedron(p);
		debug(LOG_DEBUG, DEBUG_LOG, 0, "initial curve added");
	}

	// extract union
	debug(LOG_DEBUG, DEBUG_LOG, 0, "extract the image component union");
	Nef_polyhedron	image = unioner.get_union();
	debug(LOG_DEBUG, DEBUG_LOG, 0, "base image constructed");

	// restrict everything to a box
	debug(LOG_DEBUG, DEBUG_LOG, 0, "restrict to a box");
	Build_Box	box(point(-0.1, -2, -2), point(4, 2, 2));
        Polyhedron      boxp;
        boxp.delegate(box);
	image = image * boxp;
	debug(LOG_DEBUG, DEBUG_LOG, 0, "restriction complete");

	// add axes
	if (show_axes) {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "adding axes");
		image = image + build_axes();
		debug(LOG_DEBUG, DEBUG_LOG, 0, "axes added");
	}

	// output 
	Polyhedron	P;
	debug(LOG_DEBUG, DEBUG_LOG, 0, "convert to polyhedron for output");
	image.convert_to_polyhedron(P);
	std::cout << P;

	// cut off left side of object, and save in <prefix>-left.off
	write_part(image, std::string("left"),
		Nef_polyhedron(Plane(1, 0, 0, 0), Nef_polyhedron::INCLUDED));
	write_part(image, std::string("right"), 
		Nef_polyhedron(Plane(-1, 0, 0, 0), Nef_polyhedron::INCLUDED));

	// cut off back side of object, and save in <prefix>-back.off
	write_part(image, std::string("front"),
		Nef_polyhedron(Plane(0, 1, 0, 0), Nef_polyhedron::INCLUDED));
	write_part(image, std::string("back"),
		Nef_polyhedron(Plane(0, -1, 0, 0), Nef_polyhedron::INCLUDED));

	// that's it
	debug(LOG_DEBUG, DEBUG_LOG, 0, "output complete");
	return EXIT_SUCCESS;
}

} // namespace csg

int	main(int argc, char *argv[]) {
	try {
		return csg::main(argc, argv);
	} catch (std::exception& x) {
		std::cerr << "terminated by std::exception: " << x.what()
			<< std::endl;
	} catch (...) {
		std::cerr << "terminated by unknown exception" << std::endl;
	}
}
