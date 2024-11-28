#include "./TRequest.hpp"


std::string TRequest::getMethod() {return "POST";}
std::string TRequest::getCotentLength() {return "50";};
std::string TRequest::getUri() {return "localhost:8080/cgi-bin/calendar.sh";};
std::string TRequest::getQuery() {return ("");};

std::string TRequest::getHeader(std::string value) {
	
	std::map<std::string, std::string>	header;

	header["host"] = "example.com";
	header["content-type"] = "application/x-www-form-urlencoded";
	header["content-length"] = "50";

	return (header[value]);
};