#pragma once

#include "config.hpp"

class Listen : public AConfig {
	private:
		std::string	_port;
		std::string	_host;

	public:
		Listen(std::vector<std::string> args);
		~Listen();

		std::string	getPort() const;
		std::string	getHost() const;
};