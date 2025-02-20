#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "../../../include/includes.hpp" // IWYU pragma: keep
#include <netinet/in.h>
#include <string>

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
		std::string					currServer;

		int handleNewConnection();
		int acceptClient();
	public:
		Webserv();
		~Webserv();

		void init(Http *http);
		void initSocket();
		void socketOption();
		bool runSocket();
		void run();
};

#endif