/*
 * helix.cpp
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdlib.h>
#include <stdio.h>
#include <common.h>
#include <getopt.h>
#include <Curve.h>
#include <Arrow.h>
#include <SphericalSphere.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <CGAL/Aff_transformation_3.h>
#include <debug.h>
#include <iostream>
#include <Parts.h>

namespace csg {

class Helix : public CurveFunction {
	double	_radius;
	double	_a;
public:
	Helix(double radius, double a)
		: CurveFunction(), _radius(radius), _a(a) { }

	virtual point	position(double t) const {
		return point(_radius * cos(t), _radius * sin(t), _a * t);
	}

	virtual vector	tangent(double t) const {
		return vector(-_radius * sin(t), _radius * cos(t), _a);
	}

	virtual vector	normal(double t) const {
		return vector(-cos(t), -sin(t), 0);
	}
};

Nef_polyhedron	build_helix(int steps, int phisteps, double radius) {
	// build a polygon 
	Polyhedron	p1;

	// create a curve
	Interval	threeturns(-M_PI, M_PI);
	Helix	helix(120, 100 / threeturns.length());
	Build_Curve	b1(helix, threeturns, steps, phisteps, radius);
	p1.delegate(b1);
	fprintf(stderr, "curve created\n");

	// convert to Nef polyhedra
	return Nef_polyhedron(p1);
}

Nef_polyhedron	build_frame(double a) {
	Nef_nary_union	unioner;
	double	normalize = sqrt(1 + a * a);
	double	deltax = 1 / normalize;
	double	deltay = a / normalize;
	debug(LOG_DEBUG, DEBUG_LOG, 0, "normalize = %f, (%f, %f)", normalize,
		deltax, deltay);
	{
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add X-axis");
		Polyhedron	p;
		point	end(0, 80 * deltax, 80 * deltay);
		Build_Arrow	b(point(0, 0, 0), end, 10, 16);
		p.delegate(b);
		unioner.add_polyhedron(p);
	}
	{
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add Y-axis");
		Polyhedron	p;
		Build_Arrow	b(point(0, 0, 0),
					point(-80, 0, 0), 10, 16);
		p.delegate(b);
		unioner.add_polyhedron(p);
	}
	{
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add Z-axis");
		Polyhedron	p;
		point	end(0, -80 * deltay, 80 * deltax);
		Build_Arrow	b(point(0, 0, 0), end, 10, 16);
		p.delegate(b);
		unioner.add_polyhedron(p);
	}
	{
		debug(LOG_DEBUG, DEBUG_LOG, 0, "add sphere");
		Polyhedron	p;
		Build_SphericalSphere	b(12, 20);
		p.delegate(b);
		unioner.add_polyhedron(p);
	}
	debug(LOG_DEBUG, DEBUG_LOG, 0, "extract axes union");
	Nef_polyhedron	n = unioner.get_union();

	// transform
	debug(LOG_DEBUG, DEBUG_LOG, 0, "translate the frame");
	Kernel::Vector_3	d(120, 0, 0);
	Aff_transformation	translate(CGAL::Translation(), d);
	n.transform(translate);
	return n;
}

int	main(int argc, char *argv[]) {
	int	steps = 100;
	int	phisteps = 4;
	double	radius = 5;
	int	c;
	bool	doframe = false;
	while (EOF != (c = getopt(argc, argv, "df")))
		switch (c) {
		case 'd':
			debuglevel = LOG_DEBUG;
			break;
		case 'f':
			doframe = true;
			break;
		}

	// convert to Nef polyhedra
	Nef_polyhedron	n1 = build_helix(steps, phisteps,
					radius + ((doframe) ? 1 : 0));

	// Frenet frame
	Nef_polyhedron	image;
	if (doframe) {
		Nef_polyhedron	n2 = build_frame(1. / (2. * M_PI));
		debug(LOG_DEBUG, DEBUG_LOG, 0, "compute difference");
		image = n2 - n1;
	} else {
		image = n1;
	}

	// output difference
	Polyhedron	P;
	image.convert_to_polyhedron(P);
	std::cout << P;

	// print parts
	PartWriter	pw("helix");
	pw(PartWriter::LEFT_PART, image);
	pw(PartWriter::RIGHT_PART, image);
	pw(PartWriter::FRONT_PART, image);
	pw(PartWriter::BACK_PART, image);
	pw(PartWriter::TOP_PART, image);
	pw(PartWriter::BOTTOM_PART, image);

	return EXIT_SUCCESS;
}

} // namespace csg

int	main(int argc, char *argv[]) {
	try {
		return csg::main(argc, argv);
	} catch (std::exception& x) {
		fprintf(stderr, "exception in helix: %s\n", x.what());
		return EXIT_FAILURE;
	} catch (...) {
		fprintf(stderr, "unknown exception in helix\n");
		return EXIT_FAILURE;
	}
}

