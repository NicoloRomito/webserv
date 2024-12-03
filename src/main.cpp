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

		std::string path = http->getDirective<Server>("server1")->getDirective<ErrorPage>("server1error_page4xx")->getPath();
		std::string path_1 = http->getDirective<Server>("server1")->getDirective<ErrorPage>("server1error_page5xx")->getPath();
		std::vector<int> codes = http->getDirective<Server>("server1")->getDirective<ErrorPage>("server1error_page5xx")->getCodes();

		for (std::vector<int>::iterator it = codes.begin(); it != codes.end(); it++) {
			std::cout << "CODE " << *it << std::endl;
		}

		std::cout << "PATH " << path << std::endl;

		std::cout << "PATH " << path_1 << std::endl;

		std::string path2 = http->getDirective<Server>("server2")->getDirective<ErrorPage>("server2error_page4xx")->getPath();

		std::cout << "PATH " << path2 << std::endl;

	} catch (std::exception& e) {
		std::cerr << e.what();
	}
	delete http;
}