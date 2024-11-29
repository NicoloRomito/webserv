#ifndef CGI_HPP
#define CGI_HPP

#include "../../include/includes.hpp"
#include "../../include/Directives/Server.hpp"
#include "./TRequest.hpp"
#include <vector>

class Cgi{

	private:
		std::map<std::string, std::string> _env;

	public:
		Cgi();
		Cgi(TRequest request, std::vector<std::string> cgi_pass);
};

#endif
