#ifndef CGI_HPP
#define CGI_HPP

#include "../../include/includes.hpp"
#include "../../include/Directives/Server.hpp"
#include "./TRequest.hpp"

class Cgi{

	private:
		std::map<std::string, std::string> _env;

	public:
		Cgi();
		Cgi(TRequest request, Server server);
};

#endif
