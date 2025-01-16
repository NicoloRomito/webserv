#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "../../../include/includes.hpp" // IWYU pragma: keep
#include <netinet/in.h>

class Client;

class Webserv {
	private:
		int						serverN;
		std::vector<pollfd>		pollFds;
		std::vector<int> 		serverSocket;
		std::vector<sockaddr_in> serverAddress;

	public:
		Webserv();
		void initSocket(void);
		void socketOption(int option);
		void runSocket(Http *http);
		void run();
};

void	getErrorPage(std::string& response, Response* res);

#endif