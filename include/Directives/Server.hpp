#pragma once

#include "config.hpp"
#include <sstream>

class	AConfig;

class	Server : public AConfig {
	private:
		
	public:
		Server();
		~Server();

		bool		parseErrorPage(const std::vector<std::string>& args, const std::string& directive);

		void		parse(std::stringstream& file);
};