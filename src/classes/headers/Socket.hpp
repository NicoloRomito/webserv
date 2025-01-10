#ifndef SOCKET_HPP
# define SOCKET_HPP

#include "../../../include/includes.hpp" // IWYU pragma: keep

class Socket {
	private:
		int fd;
	public:
		Socket(int option);
		int getFd();
};

#endif