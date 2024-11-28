#include "./Cgi.hpp"
#include "TRequest.hpp"
#include <string>


std::string getScriptName(TRequest request) {

	std::string delimiter = "/";
	std::string url = request.getUri();
	std::string tmp;

	while (url.find(delimiter) != std::string::npos)
	{
		tmp = url.substr(0, url.find(delimiter));
		url = url.substr(url.find(delimiter) + 1, std::string::npos);
		if (tmp == "cgi-bin/")
			return (url.substr(0, url.find(delimiter) - 1));
	}
	return ("");
};

Cgi::Cgi(TRequest request, Server server) {
	this->_env["SERVER_SOFTWARE"] = "";
	this->_env["SERVER_NAME"];
	this->_env["GATEWAY_INTERFACE"];
	this->_env["GATEWAY_INTERFACE"];
	this->_env["SERVER_PROTOCOL"];
	this->_env["SERVER_PORT"];
	this->_env["REQUEST_METHOD"];
	this->_env["PATH_INFO"];
	this->_env["PATH_TRANSLATED"];
	this->_env["SCRIPT_NAME"] = getScriptName(request);
	this->_env["QUERY_STRING"] = request.getQuery();
	this->_env["CONTENT_TYPE"] = request.getHeader("content-type");
	this->_env["CONTENT_LENGTH"] = request.getHeader("content-length");
}