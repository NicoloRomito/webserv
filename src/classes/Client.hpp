#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "../../include/includes.hpp" // IWYU pragma: keep

class Client {
	private:
		int fd;
		bool connected;
		std::string clientMsg;
		int bytesRecived;
};

#endif