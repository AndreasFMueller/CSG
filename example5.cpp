/*
 * example5.cpp -- example 5, solution of partial differential equation by
 *                            characteristics
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <fstream>
#include <common.h>
#include <debug.h>
#include <Arrow.h>
#include <Curve.h>
#include <Line.h>
#include <Cartesian.h>
#include <Box.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/Nef_nary_union_3.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>

namespace csg {

typedef	CGAL::Nef_nary_union_3<Nef_polyhedron>	Nef_nary_union;

/**
 * \brief Curve function representing Cauchy initial conditions
 */
class Sine : public CurveFunction {
public:
	Sine() : CurveFunction() { }
	virtual point	position(double t) const { return point(0, t, sin(t)); }
	virtual vector	tangent(double t) const { return vector(0, 1, cos(t)); }
	virtual vector	normal(double t) const { return vector::e3; }
};

/**
 *  \brief Function class representing the solution of the PDE
 */
class Solution : public Function {
public:
	Solution() { }
	virtual double	operator()(const double x, const double y) {
		return exp(-x) * sin(y - x * x / 2);
	}
};

/**
 * \brief Curve function class representing the characteristics
 */
class Characteristic : public CurveFunction {
	double	_y0, _z0;
public:
	Characteristic(double y0, double z0) : _y0(y0), _z0(z0) { }
	virtual point	position(double t) const {
		return point(t, 0.5 * t * t + _y0, _z0 * exp(-t));
	}
	virtual vector	tangent(double t) const {
		return vector(1, t, -_z0 * exp(-t));
	}
	virtual vector normal(double t) const {
		return vector(0, 1, _z0 * exp(-t));
	}
};

static Nef_polyhedron	build_characteristics(int phisteps, int curvesteps) {
	debug(LOG_DEBUG, DEBUG_LOG, 0, "adding characteristics");
	// the charcurves Nef_polyhedron is used to accumulate
	// the characteristic curves
	Nef_nary_union	unioner;
	Interval	interval(0, 1.9);
	for (double y0 = -M_PI / 2; y0 <= M_PI + 0.01;
		y0 += M_PI / 8) {
		debug(LOG_DEBUG, DEBUG_LOG, 0,
			"add characteristic for y0 = %f", y0);
		// build a characteristic curve vor a particular
		// y0 vaule
		Characteristic	characteristic(y0, sin(y0));
		Build_Curve	charcurve(characteristic, interval,
					curvesteps, phisteps, 0.03);
		Polyhedron	p;
		p.delegate(charcurve);
		Nef_polyhedron	n(p);

		// add the curve to the union
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add curve to union");
		unioner.add_polyhedron(n);
	}

	// add the curve to the accumulator
	debug(LOG_DEBUG, DEBUG_LOG, 0, "extract union");
	Nef_polyhedron	charcurves = unioner.get_union();

	// create a box to which we restrict the curves
	Build_Box	box(point(0, 0, -1), point(1.8, M_PI, 1.1));
	Polyhedron	boxp;
	boxp.delegate(box);

	// add characteristics curves restricted to box
	return charcurves * boxp;
}

/** 
 * \brief main function for example5
 */
int	main(int argc, char *argv[]) {
	int	c;
	int	phisteps = 4;
	int	curvesteps = 10;
	double	radius = 0.1;
	std::string	prefix("characteristics");
	bool	initialcurve = true;
	bool	solutionsurface = true;
	bool	characteristics = true;
	while (EOF != (c = getopt(argc, argv, "r:ds:p:ICSc:")))
		switch (c) {
		case 'c':
			curvesteps = atoi(optarg);
			break;
		case 'd':
			debuglevel = LOG_DEBUG;
			break;
		case 'I':
			initialcurve = false;
			break;
		case 'r':
			radius = atof(optarg);
			break;
		case 'S':
			solutionsurface = false;
			break;
		case 'C':
			characteristics = false;
			break;
		case 's':
			phisteps = atoi(optarg);
			break;
		case 'p':
			prefix = std::string(optarg);
			break;
		}

	debug(LOG_DEBUG, DEBUG_LOG, 0, "3 lines of radius %f", radius);

	// here is the Nef Polyhedron we use to store the complete picture
	Nef_polyhedron	image;

	// now add the various components, starting with the X-axis
	try {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add X-axis");
		Polyhedron	p;
		Build_Arrow	buildx(point(-0.1, 0, 0),
					point(1.9, 0, 0), 0.02, 16);
		p.delegate(buildx);
		image = image + p;
	} catch (std::exception& x) {
		fprintf(stderr, "exception while building x-axis: %s\n",
			x.what());
	} catch (...) {
		fprintf(stderr, "exception while building x-axis\n");
	}

	// adding the Y-axis
	try {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add Y-axis");
		Polyhedron	p;
		Build_Arrow	buildy(point(0, -0.1, 0),
					point(0, M_PI + 0.1, 0), 0.02, 16);
		p.delegate(buildy);
		image = image + p;
	} catch (std::exception& x) {
		fprintf(stderr, "exception while building y-axis: %s\n",
			x.what());
	} catch (...) {
		fprintf(stderr, "exception while building y-axis\n");
	}

	// adding the Z-axis
	try {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add Z-axis");
		Polyhedron	p;
		Build_Arrow	buildz(point(0, 0, -0.1), point(0, 0, 1.1),
					0.02, 16);
		p.delegate(buildz);
		image = image + p;
	} catch (std::exception& x) {
		fprintf(stderr, "exception while building z-axis: %s\n",
			x.what());
	} catch (...) {
		fprintf(stderr, "exception while building z-axis\n");
	}

	// adding the initial curve
	if (initialcurve) {
		try {
			debug(LOG_DEBUG, DEBUG_LOG, 0, "adding initial curve");
			Interval	interval(0, M_PI);
			Sine		sine;
			Build_Curve	cauchycurve(sine, interval, curvesteps,
						phisteps, 0.04);
			Polyhedron	p;
			p.delegate(cauchycurve);
			image = image + p;
		} catch (std::exception& x) {
			fprintf(stderr, "exception while adding initial curve: %s\n", x.what());
		} catch (...) {
			fprintf(stderr, "exception while adding initial curve\n");
		}
	} else {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "initial curve suppressed");
	}

	// adding the characteristics
	if (characteristics) {
		try {
			// create the characteristic curves
			Nef_polyhedron	charcurves
				= build_characteristics(phisteps, curvesteps);

			// add characteristics curves restricted to box
			debug(LOG_DEBUG, DEBUG_LOG, 0,
				"adding characteristics to image");
			image = image + charcurves;
		} catch (std::exception& x) {
			fprintf(stderr, "exception while adding characteristics: %s\n", x.what());
		} catch (...) {
			fprintf(stderr, "exception while adding characteristics\n");
		}
	} else {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "characterstics suppressed");
	}

	// adding the solution surface
	if (solutionsurface) {
		try {
			debug(LOG_DEBUG, DEBUG_LOG, 0,
				"adding solution surface");
			CartesianDomain	domain(Interval(0, 1.8),
				Interval(0, M_PI));
			Solution	sol;
			Build_CartesianFunction	b(sol, domain,
				curvesteps, curvesteps, 0.03);
			Polyhedron	p;
			p.delegate(b);
			image = image + p;
		} catch (std::exception& x) {
			fprintf(stderr, "exception while adding solution "
				"surface: %s\n", x.what());
		} catch (...) {
			fprintf(stderr, "exception while adding solution "
				"surface\n");
		}
	} else {
		debug(LOG_DEBUG, DEBUG_LOG, 0,
			"solution surface suppressed");
	}

	// output union
	Polyhedron	P;
	image.convert_to_polyhedron(P);
	std::cout << P;

	// now we have to cut along the y-z-plane, because otherwise it would
	// hardly be printable
	Nef_polyhedron	left(Plane(1, 0, 0, 0), Nef_polyhedron::INCLUDED);
	Nef_polyhedron	right(Plane(-1, 0, 0, 0), Nef_polyhedron::INCLUDED);

	// cut off left side of object, and save in <prefix>-left.off
	try {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "writing left part");
		std::string	leftname = prefix + std::string("-left.off");
		std::ofstream	leftout(leftname.c_str());
		(left * image).convert_to_polyhedron(P);
		debug(LOG_DEBUG, DEBUG_LOG, 0, "conversion to polygon complete");
		leftout << P;
		leftout.close();
	} catch (std::exception& x) {
		fprintf(stderr, "error while writing left part: %s\n",
			x.what());
	} catch (...) {
		fprintf(stderr, "error while writing left part\n");
	}

	// cut off right side of object, and save in <prefix>-right.off
	try {
		debug(LOG_DEBUG, DEBUG_LOG, 0, "writing right part");
		std::string	rightname = prefix + std::string("-right.off");
		std::ofstream	rightout(rightname.c_str());
		(right * image).convert_to_polyhedron(P);
		debug(LOG_DEBUG, DEBUG_LOG, 0, "conversion to polygon complete");
		rightout << P;
		rightout.close();
	} catch (std::exception& x) {
		fprintf(stderr, "error while writing right part: %s\n",
			x.what());
	} catch (...) {
		fprintf(stderr, "error while writing right part\n");
	}

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
