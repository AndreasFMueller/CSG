/*
 * example5.cpp -- example 5, straight lines
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <common.h>
#include <Arrow.h>
#include <Curve.h>
#include <Line.h>
#include <Cartesian.h>
#include <Box.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>

namespace csg {

class Sine : public CurveFunction {
public:
	Sine() : CurveFunction() { }
	virtual point	position(double t) const { return point(0, t, sin(t)); }
	virtual vector	tangent(double t) const { return vector(0, 1, cos(t)); }
	virtual vector	normal(double t) const { return vector::e3; }
};

class Solution : public Function {
public:
	Solution() { }
	virtual double	operator()(const double x, const double y) {
		return exp(-x) * sin(y - x * x / 2);
	}
};

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

/** 
 * \brief main function for example5
 */
int	main(int argc, char *argv[]) {
	int	c;
	int	phisteps = 4;
	double	radius = 0.1;
	while (EOF != (c = getopt(argc, argv, "r:ds:")))
		switch (c) {
		case 'd':
			debug++;
			break;
		case 'r':
			radius = atof(optarg);
			break;
		case 's':
			phisteps = atoi(optarg);
			break;
		}

	if (debug) {
		fprintf(stderr, "%s:%d: 3 lines of radius %f\n",
			__FILE__, __LINE__, radius);
	}

	// here is the Nef Polyhedron we use to store the complete picture
	Nef_polyhedron	image;

	// now add the various components
	try {
		if (debug) {
			fprintf(stderr, "%s:%d: add X-axis\n",
				__FILE__, __LINE__);
		}
		Polyhedron	p;
		Build_Arrow	buildx(point(-0.1, 0, 0),
					point(1.9, 0, 0), 0.02, 16);
		p.delegate(buildx);
		image = image + p;
	} catch (...) {
		fprintf(stderr, "exception while building x-axis\n");
	}
	try {
		if (debug) {
			fprintf(stderr, "%s:%d: add Y-axis\n",
				__FILE__, __LINE__);
		}
		Polyhedron	p;
		Build_Arrow	buildy(point(0, -0.1, 0),
					point(0, M_PI + 0.1, 0), 0.02, 16);
		p.delegate(buildy);
		image = image + p;
	} catch (...) {
		fprintf(stderr, "exception while building y-axis\n");
	}
	try {
		if (debug) {
			fprintf(stderr, "%s:%d: add Z-axis\n",
				__FILE__, __LINE__);
		}
		Polyhedron	p;
		Build_Arrow	buildz(point(0, 0, -0.1), point(0, 0, 1.1),
					0.02, 16);
		p.delegate(buildz);
		image = image + p;
	} catch (...) {
		fprintf(stderr, "exception while building z-axis\n");
	}

	try {
		if (debug) {
			fprintf(stderr, "%s:%d: adding initial curve\n",
				__FILE__, __LINE__);
		}
		Interval	interval(0, M_PI);
		Sine		sine;
		Build_Curve	cauchycurve(sine, interval, 50, phisteps, 0.04);
		Polyhedron	p;
		p.delegate(cauchycurve);
		image = image + p;
	} catch (...) {
		fprintf(stderr, "exception while adding initial curve\n");
	}

	try {
		if (debug) {
			fprintf(stderr, "%s:%d: adding solution surface\n",
				__FILE__, __LINE__);
		}
		CartesianDomain	domain(Interval(0, 1.8), Interval(0, M_PI));
		Solution	sol;
		Build_CartesianFunction	b(sol, domain, 50, 50, 0.03);
		Polyhedron	p;
		p.delegate(b);
		image = image + p;
	} catch (...) {
		fprintf(stderr, "exception while adding solution surface\n");
	}

	try {
		if (debug) {
			fprintf(stderr, "%s:%d: adding characteristics\n",
				__FILE__, __LINE__);
		}
		Nef_polyhedron	charcurves;
		Interval	interval(0, 1.9);
		for (double y0 = -M_PI / 2; y0 <= M_PI + 0.01; y0 += M_PI / 8) {
			Characteristic	characteristic(y0, sin(y0));
			Build_Curve	charcurve(characteristic, interval,
						50, phisteps, 0.03);
			Polyhedron	p;
			p.delegate(charcurve);
			charcurves = charcurves + p;
		}
		Build_Box	box(point(0, 0, -1), point(1.8, M_PI, 1.1));
		Polyhedron	p;
		p.delegate(box);
		image = image + charcurves * p;
	} catch (...) {
		fprintf(stderr, "exception while adding characteristics\n");
	}

	// output union
	Polyhedron	P;
	image.convert_to_polyhedron(P);
	std::cout << P;

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
