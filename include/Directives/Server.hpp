#pragma once

#include "config.hpp"
#include <sstream>

class	AConfig;

class	Server : public AConfig {
	private:

	public:
		Server(std::stringstream& file);
		~Server();

		// getDirective;
		// take config file line by line in the while loops for exception handling, using global variable -> ExceptionLine
		AConfig*	createBlock(const std::string& directive, std::stringstream& file);

		const AConfig*	getDirective(const std::string& directiveName) const;
};