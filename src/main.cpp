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

		const Server *server = dynamic_cast<const Server *>(http->getDirective("server1"));
		const Server *server2 = dynamic_cast<const Server *>(http->getDirective("server2"));

		const Listen *listen = dynamic_cast<const Listen *>(server->getDirective("listen"));
		const Listen *listen2 = dynamic_cast<const Listen *>(server2->getDirective("listen"));

		std::cout << "Port -> " << listen->getPort() << std::endl;
		std::cout << "IP: " << listen->getIp() << std::endl;

		std::cout << "Port -> " << listen2->getPort() << std::endl;
		std::cout << "IP: " << listen2->getIp() << std::endl;

		delete http;
	} catch (std::exception& e) {
		std::cerr << e.what();
	}
}