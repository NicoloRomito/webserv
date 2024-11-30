#include "../include/includes.hpp"
#include "../include/Directives/Http.hpp"
#include "../include/Directives/Server.hpp"
#include "../include/Directives/config.hpp"
#include "../include/Directives/Listen.hpp"
#include <exception>
#include <fstream>

int	main(int ac, char **av)
{
	if (ac != 2) {
		std::cerr << "Error: bad input\n";
	}

	std::stringstream fileStream;

	try {
		startParsing(av[1], fileStream);
		Http *http = new Http(fileStream);

		std::string ip = http->getDirective<Server>("server1")->getDirective<Listen>("listen")->getIp();

		std::cout << "IP: " << ip << std::endl;

		delete http;
	} catch (std::exception& e) {
		std::cerr << e.what();
	}
}