#include "../include/includes.hpp"
#include "../include/config.hpp"
#include <exception>

int	main(int ac, char **av)
{
	if (ac != 2) {
		std::cerr << "Error: bad input\n";
	}

	Config	c;

	try {
		c.startParsingFile(av[1]);
	} catch (std::exception& e) {
		std::cerr << e.what();
	}
}