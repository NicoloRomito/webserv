#pragma once

#include "config.hpp"

class	AConfig;

class Listen : public AConfig {
	private:
		std::string	_port;
		std::string	_host;

	public:
		Listen(const std::vector<std::string>& args);
		~Listen();

		std::string	getPort() const;
		std::string	getHost() const;
};