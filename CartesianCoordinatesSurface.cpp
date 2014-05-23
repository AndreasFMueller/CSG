/*
 * CartesianCoordinatesSurface.cpp -- Create a surface in cartesian coordinates
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <CartesianCoordinatesSurface.h>

#include <CGAL/Polyhedron_incremental_builder_3.h>

namespace csg {

int	Build_CartesianCoordinatesSurface::vertex(const int x, const int y) const {
	return 2 * (x * (_ysteps + 1) + y);
}

void	Build_CartesianCoordinatesSurface::operator()(Polyhedron::HalfedgeDS& hds) {
	CGAL::Polyhedron_incremental_builder_3<Polyhedron::HalfedgeDS>	B(hds, true);
	B.begin_surface(0, 0, 0);
	double	deltax = _f.xrange().length() / _xsteps;
	double	deltay = _f.yrange().length() / _ysteps;
	for (int x = 0; x <= _xsteps; x++) {
		double	_x = _f.xrange().min() + x * deltax;
		for (int y = 0; y <= _ysteps; y++) {
			double	_y = _f.yrange().min() + y * deltay;
			double	z = _f(_x, _y);
			add_vertex(B, _x, _y, z + _h/2);
			add_vertex(B, _x, _y, z - _h/2);
		}
	}
	for (int x = 0; x < _xsteps; x++) {
		if (debug) {
			fprintf(stderr, "%s:%d: x = %d\n",
				__FILE__, __LINE__, x);
		}
		for (int y = 0; y < _ysteps; y++) {
			add_facet(B, 
				vertex(x,     y    )    ,
				vertex(x + 1, y    )    ,
				vertex(x,     y + 1)    );

			add_facet(B,
				vertex(x,     y    ) + 1,
				vertex(x,     y + 1) + 1,
				vertex(x + 1, y    ) + 1);
			
			add_facet(B,
				vertex(x,     y + 1)    ,
				vertex(x + 1, y    )    ,
				vertex(x + 1, y + 1)    );
			
			add_facet(B,
				vertex(x,     y + 1) + 1,
				vertex(x + 1, y + 1) + 1,
				vertex(x + 1, y    ) + 1);
		}
	}
	for (int x = 0; x < _xsteps; x++) {
		add_facet(B,
			vertex(x    , 0)    ,
			vertex(x    , 0) + 1,
			vertex(x + 1, 0)    );

		add_facet(B,
			vertex(x + 1, 0)    ,
			vertex(x    , 0) + 1,
			vertex(x + 1, 0) + 1);

		add_facet(B,
			vertex(x    , _ysteps),
			vertex(x + 1, _ysteps),
			vertex(x    , _ysteps) + 1);

		add_facet(B,
			vertex(x + 1, _ysteps),
			vertex(x + 1, _ysteps) + 1,
			vertex(x    , _ysteps) + 1);
	}
	for (int y = 0; y < _ysteps; y++) {
		add_facet(B,
			vertex(0, y    )    ,
			vertex(0, y + 1)    ,
			vertex(0, y    ) + 1);

		add_facet(B,
			vertex(0, y + 1)    ,
			vertex(0, y + 1) + 1,
			vertex(0, y    ) + 1);

		add_facet(B,
			vertex(_xsteps, y    )    ,
			vertex(_xsteps, y    ) + 1,
			vertex(_xsteps, y + 1)    );

		add_facet(B,
			vertex(_xsteps, y    ) + 1,
			vertex(_xsteps, y + 1) + 1,
			vertex(_xsteps, y + 1)    );
	}
	B.end_surface();
}

} // namespace csg
