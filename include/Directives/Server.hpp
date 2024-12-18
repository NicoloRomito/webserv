#pragma once

#include "config.hpp"
#include <sstream>

class	AConfig;

class	Server : public AConfig {
	private:
		int	_nListen;
		
	public:
		Server();
		~Server();

		int	getNumberOfListen() const;

		bool		parseErrorPage(const std::vector<std::string>& args, const std::string& directive);
		void		parseListen(const std::vector<std::string>& args, const std::string& directive, int listenN);
		void		parse(std::stringstream& file);
};