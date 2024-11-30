#pragma once

#include "config.hpp"
#include <sstream>

class	AConfig;

class	Http : public AConfig {
	private:

	public:
		Http(std::stringstream& file);
		~Http();

		AConfig*	createBlock(const std::string& directive, std::stringstream& file);

};