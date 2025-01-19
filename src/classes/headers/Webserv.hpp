#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "../../../include/includes.hpp" // IWYU pragma: keep
#include <netinet/in.h>

class Client;

class Webserv {
	private:
		int							portN;
		int							option;
		Http						*http;
		std::vector<pollfd>			pollFds;
		std::vector<int>			serverSocket;
		std::vector<sockaddr_in>	serverAddress;
		std::map<int, std::string>	listenMap;
	public:
		Webserv();
		void init(Http *http);
		void initSocket();
		void socketOption();
		void runSocket();
		void run();
};

#endif