#pragma once

#include "config.hpp"

class	AConfig;

class Listen : public AConfig {
	private:
		std::string	_port;
		std::string	_ip;

	public:
		Listen(const std::vector<std::string>& args);
		~Listen();

		std::string	getPort() const;
		std::string	getIp() const;
};