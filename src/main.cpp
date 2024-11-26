#include "../include/includes.hpp"
#include "../include/Directives/config.hpp"
#include <exception>

int	main(int ac, char **av)
{
	if (ac != 2) {
		std::cerr << "Error: bad input\n";
	}

	try {
		startParsing(av[1]);
		// Http *http = new Http(stream); 
	} catch (std::exception& e) {
		std::cerr << e.what();
	}
}