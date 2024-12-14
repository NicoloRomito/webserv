#include "../headers/Request.hpp"
#include <cctype>
#include <cstddef>
#include <string>

Request::Request():  uri(""), url(""),
					header(""), body(""), method(""), 
					version(""), path(""), host("") {}

Request::~Request() {}

void trimString(std::string &str) {
	while (isspace(str[0]))
		str.erase(0, 1);
	while (isspace(str[str.size() - 1]))
		str.erase(str.size() - 1, 1);

}

void printVec(std::vector<std::string> vec) {
	std::cout << "---VECTOR PRINT---\n";
	for (size_t i = 0; i < vec.size(); i++) {
		std::cout << i << ": " << vec[i] << '\n';
	}
	std::cout << "---END---\n";
}

std::vector<std::string> getBasicInfo(std::string buffer, char delim) {
	std::vector<std::string> splitReq;
	size_t						i = 0;
	std::string				word;

	trimString(buffer);
	while (i < buffer.size()) {
		while (buffer[i] && buffer[i] != delim) {
			word.push_back(buffer[i]);
			i++;
		}
		while (buffer[i] && buffer[i] == delim) {
			i++;
		}
		splitReq.push_back(word);
		word.clear();
	}
	return splitReq;
}

void Request::parseRequest(std::string buffer) {
	std::vector<std::string> splitReq;

	std::string temp;

	temp.insert(0, buffer, 0, buffer.find('\n'));
	splitReq = getBasicInfo(temp, 32);

	printVec(splitReq);
	if (splitReq.size() < 3) {
		error("client error");
	}
	this->method = splitReq[0];
	this->path = splitReq[1];
	this->version = splitReq[2];
}

std::string Request::getMethod() const {
	return this->method;
}

std::string Request::getVersion() const {
	return this->version;
}

std::string Request::getPath() const {
	return this->path;
}

std::string Request::getUri() const {
	return "http://" + this->host + this->path;
}

void	Request::setCgiOutput(const std::string toSet)
{
	this->_cgiOutput = toSet;
}

std::string Request::getCgiOutput() const {return this->_cgiOutput;}