#include "include/Directives/Listen.hpp"
#include "include/Directives/Server.hpp"
#include "include/includes.hpp"
#include "include/includeClasses.hpp"
#include "src/classes/headers/Webserv.hpp"

// * NOTE: This is a list of the directives that need to be called with numbers
// * at the end of the directive name. Here is the list:
// * Server -> server1, server2, server3, ...
// * If you have to call a directive inside server block, you have to call "server1root", "server1error_page4xx", "server1error_page5xx", ...
// * Location -> location1, location2, location3, ...
// * If you have to call a directive inside location block, you have to call "location1root", "location1error_page4xx", "location1error_page5xx", ...
// * Listen -> listen1, listen2, listen3, ...
// * ErrorPage -> server1error_page4xx, server1error_page5xx, ...

// ! NOTE: DEFAULT DIRECTIVES ARE:
// * Http: client_max_body_size;
// * Server: listen, server_name, error_page4xx, error_page5xx;
// * Location: cgi_pass, root, index, autoindex;

// ! NOTE: getDirective returns NULL if the directive does not exist

int QUIT = 0;

int checkSocketAvailable(std::vector<pollfd> pollFds, int serverN) {
	for (int i = 0; i < serverN; i++) {
		if (pollFds[i].revents & POLLIN)
			return i;
	}
	return -1;
}

int main(int ac, char **av) {
	if (ac != 2) {
		std::cerr << "Usage: ./webserv <config_file>" << std::endl;
		return 0;
	}
	std::stringstream fileStream;
	Http *http = new Http();
	Webserv	webserv;

	try {
		startParsing(av[1], fileStream);
		http->parse(fileStream);
	} catch (std::exception& e) {
		std::cerr << e.what();
		delete http;
		return 0;
	}

	webserv.init(http);
	webserv.initSocket();
	webserv.socketOption();
	webserv.runSocket();
	webserv.run();
	//todo thow an error on init scoket

	delete http;
	std::cout << "[SERVER MESSAGE] -> SERVER IS NOW SHUT DOWN\n";
	return 0;
}
