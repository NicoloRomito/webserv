#pragma once

#include "config.hpp"
#include <sstream>

class	AConfig;

class	Server : public AConfig {
	private:

	public:
		Server(std::stringstream& file);
		~Server();

		AConfig*	createBlock(const std::string& directive, std::stringstream& file);
};