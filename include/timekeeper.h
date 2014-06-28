/*
 * timekeeper.h -- timekeeper class for time measurements
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */

namespace csg {

class timekeeper {
	double	start;
	double	start_cpu;
	double	gettime() const;
	double	getcpu() const;
public:
	timekeeper();
	double	starttime() const;
	double	cpu() const;
	double	elapsed() const;
};

} // namespace csg
