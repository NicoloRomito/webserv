#include "../include/includes.hpp"
#include "../include/Directives/Http.hpp"
#include "../include/Directives/Server.hpp"
#include "../include/Directives/Location.hpp"
#include "../include/Directives/ServerName.hpp"
#include "../include/Directives/Root.hpp"
#include "../include/Directives/Index.hpp"
#include "../include/Directives/ErrorPage.hpp"
#include "../include/Directives/ClientMaxBodySize.hpp"
#include "../include/Directives/Autoindex.hpp"
#include "../include/Directives/CgiPass.hpp"
#include "../include/Directives/config.hpp"
#include "../include/Directives/Listen.hpp"
#include <exception>
#include <fstream>
#include <string>
#include <vector>

// * NOTE: This is a list of the directives that need to be called with numbers
// * at the end of the directive name. Here is the list:
// * Server -> server1, server2, server3, ...
// * If you have to call a directive inside server block, you have to call "server1root", "server1error_page4xx", "server1error_page5xx", ...
// * Location -> location1, location2, location3, ...
// * If you have to call a directive inside location block, you have to call "location1root", "location1error_page4xx", "location1error_page5xx", ...
// * Listen -> listen1, listen2, listen3, ...
// * ErrorPage -> server1error_page4xx, server1error_page5xx, ...

// ? Is it okay if the configuration map is empty and when you call getDirective it throws an exception? Because it's empty.

int	main(int ac, char **av)
{
	if (ac != 2) {
		std::cerr << "Error: bad input\n";
	}

	std::stringstream fileStream;
	Http *http = new Http();

	try {
		startParsing(av[1], fileStream);
		http->parse(fileStream);

		std::string ip = http->getDirective<Server>("server1")->getDirective<Listen>("listen1")->getPort();

		std::string path_1 = http->getDirective<Server>("server1")->getDirective<Listen>("listen2")->getPort();
		// std::vector<int> codes = http->getDirective<Server>("server1")->getDirective<ErrorPage>("server1error_page5xx")->getCodes();

		// for (std::vector<int>::iterator it = codes.begin(); it != codes.end(); it++) {
		// 	std::cout << "CODE " << *it << std::endl;
		// }

		std::cout << "PORT " << ip << std::endl;

		std::cout << "PORT " << path_1 << std::endl;

		// std::string path2 = http->getDirective<Server>("server2")->getDirective<ErrorPage>("server2error_page4xx")->getPath();

		// std::cout << "PATH " << path2 << std::endl;

	} catch (std::exception& e) {
		std::cerr << e.what();
	}
	delete http;
}