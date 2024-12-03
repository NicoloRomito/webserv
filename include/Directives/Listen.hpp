#pragma once

#include "config.hpp"

class	AConfig;

class Listen : public AConfig {
	private:
		std::string	_port;
		std::string	_ip;

	public:
		Listen();
		~Listen();

		const std::string&	getPort() const;
		const std::string&	getIp() const;

		void				parseDirective(const std::vector<std::string>& args);
};