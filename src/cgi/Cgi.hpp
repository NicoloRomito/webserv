#ifndef CGI_HPP
#define CGI_HPP

#include "../../include/includes.hpp"
#include "../../include/Directives/Server.hpp"
#include "./TRequest.hpp"
#include <string>
#include <vector>

class TRequest;

class Cgi{

	private:
		std::map<std::string, std::string> _env;
		std::string _cgiPath;
		char** _argv;
		char** _envp;
	

	public:
		Cgi();
		Cgi(TRequest & request, std::vector<std::string> & cgi_pass);
		char** createEnvp();
		std::string	getCgiPath();
		char** getEnvp();
		char** getArgv();
};

#endif
