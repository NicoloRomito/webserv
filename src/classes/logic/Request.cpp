#include "../headers/Request.hpp"
#include <cstddef>

Request::Request():  uri(""), url(""),
					header(""), body(""), method(""), 
					version(""), path(""), host("") {}

Request::~Request() {}

std::vector<std::string> split(std::string buffer, char delim) {
	std::vector<std::string> splitReq;
	size_t						i = 0;
	int						found;
	std::string				word;

	while (buffer[0] == 32)
		buffer.erase(0, 1);
	while (buffer[buffer.size() - 1] == 32)
		buffer.erase(buffer.size() - 1, 1);

	while (i < buffer.size()) {
		while (buffer[i] == delim)
			i++;
		found = buffer.find(delim, i);
		if (found == -1)
			found = buffer.size();
		word.insert(0, buffer, i, found - i);
		std::cout << word << '\n';
		splitReq.push_back(word);
		i = found;
		word.clear();
	}
	return splitReq;
}

void Request::parseRequest(std::string buffer) {
	std::vector<std::string> splitReq;


	splitReq = split(buffer, 32);


	std::cout << "-----splitted------\n";
	std::cout << splitReq.size() << '\n';
	for (size_t i = 0; i < splitReq.size(); i++) {
		std::cout << splitReq[i] << '\n';
	}
	this->path = splitReq[1];
	std::cout << "-----end------\n";

	// this->method = splitReq[0];
	// this->path = splitReq[1];
	// this->version = splitReq[2].erase(splitReq[2].find('\r'), 1);

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