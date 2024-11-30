#include "./Cgi.hpp"
#include "TRequest.hpp"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <unistd.h>
#include <vector>


std::string getScriptName(TRequest request) {

	std::string delimiter = "/";
	std::string url = request.getPath();
	std::string tmp;

	while (url.find(delimiter) != std::string::npos)
	{
		tmp = url.substr(0, url.find(delimiter));
		url = url.substr(url.find(delimiter) + 1, std::string::npos);
		if (tmp == "cgi-bin")
			if (url != "")
				return (url.substr(0, url.find(delimiter) - 1));
	}
	return ("");
};

std::string getPort() {
	return ("8081");
}

std::string getScriptPath(std::string url) 
{
	std::string ret;
	(void) url;

	ret  = "/cgi-bin/calendar.sh";
	return (ret); 
}

// void checkInterpreter(std::vector<std::string> cgi_pass, std::string path) 
// {
// 	std::fstream file;

// 	if (!)
// 	throw "int";
// }

char** Cgi::getEnvp() {

	return (this->_envp);
}

char** Cgi::getArgv() {
	return (this->_argv);
}

Cgi::Cgi(TRequest & request, std::vector<std::string> & cgi_pass) {
	(void) cgi_pass;
	std::string relFilePath;

	this->_env["AUTH_TYPE"] = "";
	this->_env["SERVER_SOFTWARE"] = "WEBSERV";
	this->_env["SERVER_NAME"] = "localhost";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["SERVER_PROTOCOL"] = "http/1.1";
	this->_env["SERVER_PORT"] = getPort();
	this->_env["REQUEST_METHOD"] = request.getMethod();
	this->_cgiPath = getScriptPath(request.getPath());
	this->_env["PATH_INFO"] = getScriptPath(request.getPath());
	this->_env["PATH_TRANSLATED"] = getScriptPath(request.getPath());
	this->_env["SCRIPT_NAME"] = getScriptName(request);
	if (request.getMethod() == "POST")
	{	
		this->_env["CONTENT_TYPE"] = request.getHeader("content-type");
		this->_env["CONTENT_LENGTH"] = request.getCotentLength();
	}
	this->_env["QUERY_STRING"] = "";

	this->_envp = createEnvp();
	this->_argv = new char*[2];
	relFilePath = "." + getScriptPath(request.getPath());
	this->_argv[0] = strdup(relFilePath.c_str());
	this->_argv[1] = 0;
	// checkInterpreter(cgi_pass, request.getPath());
}

std::string Cgi::getCgiPath() {return this->_cgiPath;}

char**	Cgi::createEnvp()
{
	std::vector<std::pair<std::string, std::string> > vect;
	char** env_matrix = new char*[_env.size() + 1];
	env_matrix[_env.size()] = 0;
	std::string tmp;

	std::copy(this->_env.begin(), this->_env.end(), std::back_inserter(vect));

	
	for (size_t i = 0; i < _env.size(); i++)
	{
		tmp = (vect[i].first + "=" + vect[i].second);

		env_matrix[i] = strdup(tmp.c_str());
	}

	return (env_matrix);
}