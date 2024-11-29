#include "./Cgi.hpp"
#include "TRequest.hpp"
#include <fstream>
#include <string>
#include <vector>


std::string getScriptName(TRequest request) {

	std::string delimiter = "/";
	std::string url = request.getPath();
	std::string tmp;

	while (url.find(delimiter) != std::string::npos)
	{
		tmp = url.substr(0, url.find(delimiter));
		url = url.substr(url.find(delimiter) + 1, std::string::npos);
		if (tmp == "cgi-bin/")
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

	ret  = "/cgi-bin/calendar.sh";
	return (ret); 
}

void checkInterpreter(std::vector<std::string> cgi_pass, std::string path) 
{
	std::fstream file;

	if (!)
	throw "int";
}

Cgi::Cgi(TRequest request, std::vector<std::string> cgi_pass) {
	this->_env["SERVER_SOFTWARE"] = "WEBSERV";
	this->_env["SERVER_NAME"] = "localhost";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["SERVER_PROTOCOL"] = "http/1.1";
	this->_env["SERVER_PORT"] = getPort();
	this->_env["REQUEST_METHOD"] = request.getMethod();
	this->_env["PATH_INFO"] = getScriptPath(request.getPath());
	this->_env["PATH_TRANSLATED"] = getScriptPath(request.getPath());
	this->_env["SCRIPT_NAME"] = getScriptName(request);
	this->_env["CONTENT_TYPE"] = request.getHeader("content-type");
	this->_env["CONTENT_LENGTH"] = request.getCotentLength();

	checkInterpreter(cgi_pass, request.getPath());
}