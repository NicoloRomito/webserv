#pragma once

#include "config.hpp"
#include <sstream>

class	AConfig;

class	Http : public AConfig {
	private:
		int		_serverN;

	public:
		Http();
		~Http();

		void	parse(std::stringstream& file);
		void	compareServerPorts();

		int		getServerN() const;
		std::string getServerName(const std::string& host);
		std::string	getLocationName(const std::string& path, const std::string& serverName);
};