#ifndef CGI_HPP
#define CGI_HPP

#include "../../include/includes.hpp"
#include "../../include/Directives/Server.hpp"
#include "../classes/headers/Request.hpp"
#include "../classes/headers/Response.hpp"
#include "../../include/Errors.hpp"
#include <string>

class Cgi{

	private:
		std::map<std::string, std::string> _env;
		std::string _cgiPath;
		char** _argv;
		char** _envp;
		char** _createArgv(Request &req, Response *res);

	public:
		Cgi(Request & request, Response* res);
		~Cgi();
		char** createEnvp();
		void executeCgi(Response *res, Request *req, int& statusCode);
		std::string	getCgiPath();
		char** getArgv();
};


//Utils is file CgiUtils.cpp
void unlinkCgi();
char* getScriptAbsPath(std::string path, std::string root);
std::string buildCgiOutputPath(int idx);
int countCharInstances(const std::string query, char delim);
char* getValueFromQuery(std::string query);
int cgiHandler(Request* req, int& statusCode, std::string& response, Response* res);

#endif
