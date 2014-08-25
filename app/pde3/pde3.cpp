/*
 * pde3.cpp -- 1st order PDE without solutions
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdlib.h>
#include <parameters.h>
#include <debug.h>
#include <common.h>

namespace csg {

bool	support_enable = true;
bool	axes_enable = true;
bool	initial_enable = true;
bool	characteristics_enable = true;
bool	solution_enable = true;

int	main(int argc, char *argv[]) {
	int	c;
	while (EOF != (c = getopt(argc, argv, "dPXICS")))
		switch (c) {
		case 'd':
			if (debuglevel == LOG_DEBUG) {
				debuglevel++;
			} else {
				debuglevel = LOG_DEBUG;
			}
			break;
		case 'P':
			support_enable = false;
			break;
		case 'X':
			axes_enable = false;
			break;
		case 'I':
			initial_enable = false;
			break;
		case 'C':
			characteristics_enable = false;
			break;
		case 'S':
			solution_enable = false;
			break;
		}
	return EXIT_SUCCESS;
}

} // namespace csg

int	main(int argc, char *argv[]) {
	try {
		return csg::main(argc, argv);
	} catch (std::exception& x) {
		std::cerr << "terminated by exception: " << x.what() << std::endl;
	} catch (...) {
		std::cerr << "terminated by unknown exception" << std::endl;
	}
	return EXIT_FAILURE;
}
