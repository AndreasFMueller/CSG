/*
 * example1.cpp -- example 1
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <common.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Aff_transformation_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <SphericalSphere.h>
#include <SphericalSurface.h>
#include <Cartesian.h>
#include <Polar.h>

namespace csg {

double	radius = 1;

/**
 * \brief Sine function
 */
class	Sine : public SphericalFunction {
	double	_radius;
public:
	Sine(double radius) : _radius(radius) { }
	virtual double	operator()(const double theta, const double phi) {
		return _radius * (0.1 + sin(theta)) * (1 + 0.1 * sin(10 * phi));
	}
};

/**
 * \brief Parabola
 */
class	Parabola : public Function {
public:
	Parabola() { }
	double	operator()(const double x, const double y) {
		return (x * x + y * y) / 20;
	}
};

/**
 * \brief Sine3 function for Polar example
 */
class	Sine3 : public Function {
public:
	Sine3() { }
	double	operator()(const double r, const double phi) {
		return r * r * r * sin(3 * phi) / 25.;
	}
};

/** 
 * \brief main function for example1
 */
int	main(int argc, char *argv[]) {
	int	c;
	int	steps = 10;
	while (EOF != (c = getopt(argc, argv, "r:dn:")))
		switch (c) {
		case 'd':
			debug++;
			break;
		case 'r':
			radius = atof(optarg);
			break;
		case 'n':
			steps = atoi(optarg);
			break;
		}

	if (debug) {
		fprintf(stderr, "%s:%d: sphere of radius %f\n",
			__FILE__, __LINE__, radius);
	}

	// build a polygon 
	Polyhedron	p1, p2, p3, p4;

	// create spherical surfaces
	Build_SphericalSphere	b1(radius, steps);
	p1.delegate(b1);
	fprintf(stderr, "sphere created\n");

	Sine	sine(radius);
	Build_SphericalSurface	b2(sine, 4 * steps);
	p2.delegate(b2);

	// cartesian surfaces
	Parabola	parabola;
	CartesianDomain	cartesiandomain(Interval(-10, 10), Interval(-10, 10));
	Build_CartesianFunction	b3(parabola, cartesiandomain,
					2 * steps, 2 * steps, 1);
	p3.delegate(b3);

	// polar surfaces
	Sine3	sine3;
	PolarDomain	polardomain(Interval(1,5), Interval2Pi);
	Build_PolarFunction	b4(sine3, polardomain, steps, 2 * steps, 0.1);
	p4.delegate(b4);

	// compute translation
	typedef	CGAL::Vector_3<Kernel>	Vector;
	Vector	d(radius, 0, 0);
	Aff_transformation	translate(CGAL::Translation(), d);

	// convert to Nef polyhedra
	Nef_polyhedron	n1(p1);
	fprintf(stderr, "Nef_polyhedron created\n");
	Nef_polyhedron	n2(p2);
	Nef_polyhedron	n3(p3);
	Nef_polyhedron	n4(p4);

	n2.transform(translate);
	Nef_polyhedron	difference(n1 - n4);

	// output difference
	Polyhedron	P;
	difference.convert_to_polyhedron(P);
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
