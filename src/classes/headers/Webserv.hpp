#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "../../../include/includes.hpp" // IWYU pragma: keep

class Client;

class Webserv {
	private:
		int serverSocket;
		std::vector<pollfd> pollFds;
		std::vector<Client> clients;
	public:
		void run();
};

void	getErrorPage(std::string& response, Response* res);

#endif