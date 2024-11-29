#include "../headers/Request.hpp"
#include <cstddef>

Request::Request():  uri(""), url(""),
					header(""), body(""), method(""), version(""), path("") {}

Request::~Request() {}

std::vector<std::string> split(std::string buffer) {
	std::vector<std::string> splitReq;
	size_t						i;
	int						skip;
	std::string				key;

	while (i < buffer.size() && buffer[i] != '\n') {
		while (buffer[i] == 32) {
			i++;
		}
		skip = buffer.find(32, i);
		if (skip == -1)
			skip = buffer.find('\n', i);
		key.insert(0, buffer, i, skip - i);
		std::cout << key << '\n';
		splitReq.push_back(key);
		i += key.length();
		key.clear();
		if (skip == -1) skip = 1;
		std::cout << skip << '\n';
	}
}

void Request::parseRequest(std::string buffer) {
	std::vector<std::string> splitReq;


	splitReq = split(buffer);
	this->method = splitReq[0];
	this->path = splitReq[1];
	this->version = splitReq[2];
	// std::cout << buffer << '\n';
	
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