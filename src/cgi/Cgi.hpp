#ifndef CGI_HPP
#define CGI_HPP

#include "../../include/includes.hpp"
#include "../../include/Directives/Server.hpp"
#include "../classes/headers/Request.hpp"
#include <string>

class TRequest;

class Cgi{

	private:
		std::map<std::string, std::string> _env;
		std::string _cgiPath;
		char** _argv;
		char** _envp;
		char** _createArgv(Request &req);

	public:
		Cgi(Request & request);
		~Cgi();
		char** createEnvp();
		void executeCgi(Request *req);
		std::string	getCgiPath();
		char** getArgv();
};

void unlinkCgi();

#endif
