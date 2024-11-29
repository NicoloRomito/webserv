#include "./TRequest.hpp"


std::string TRequest::getMethod() {return "POST";}
std::string TRequest::getCotentLength() {return "50";};
std::string TRequest::getPath() {return "/cgi-bin/calendar.sh";};
std::string TRequest::getUrl() {return "http:/localhost:8080/cgi-bin/calendar.sh";};
std::string TRequest::getQuery() {return ("");};
std::string TRequest::getHeader(std::string value) {
	
	std::map<std::string, std::string>	header;

	header["host"] = "example.com";
	header["content-type"] = "text/x-shellscript";
	header["content-length"] = "2000";

	return (header[value]);
};