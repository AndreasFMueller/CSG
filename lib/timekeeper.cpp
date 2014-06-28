/*
 * timekeeper.cpp -- time keeper class
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <timekeeper.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>

namespace csg {

timekeeper::timekeeper() {
	start = gettime();
	start_cpu = getcpu();
}

static double	doubletime(const struct timeval& tv) {
	return tv.tv_sec + 0.000001 * tv.tv_usec;
}

double	timekeeper::gettime() const {
	struct timeval	tv;
	gettimeofday(&tv, NULL);
	return doubletime(tv);
}

double	timekeeper::getcpu() const {
	struct rusage	ru;
	getrusage(RUSAGE_SELF, &ru);
	return doubletime(ru.ru_utime) + doubletime(ru.ru_stime);
}

double	timekeeper::starttime() const {
	return start;
}

double	timekeeper::cpu() const {
	return getcpu() - start_cpu;
}

double	timekeeper::elapsed() const {
	return gettime() - start;
}

} // namespace csg
