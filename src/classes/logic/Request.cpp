#include "../headers/Request.hpp"
#include <cctype>
#include <cstddef>
#include <string>

Request::Request():  uri(""), url(""),
					header(), body(), method(""), 
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

std::map<std::string, std::string> setMap(std::string &buffer) {
	std::map <std::string, std::string> map;
	std::string key, value;
	std::string word;
	int found;
	size_t j = 0;

	while (!buffer.empty()) {
		if (buffer[0] == '{')
			return map;
		found = buffer.find('\n');
		word.insert(0, buffer, 0, found);
		if (word[0] == '\r') {
			buffer.erase(0, buffer.length() -1);
			return map;
		}
		buffer.erase(0, word.length() + 1);
		trimString(word);
		while (word[j] != ':') {
			key.append(1, word[j]);
			j++;
		}
		j++;
		while (j < word.size()) {
			value.append(1, word[j]);
			j++;
		}
		map[key] = value;
		word.clear();
		key.clear();
		value.clear();
		j = 0;
	}
	return map;
}

typedef std::map<std::string, std::string>::iterator mapIt;

void printMap(std::map<std::string, std::string> map) {
	std::map<std::string, std::string>::iterator it = map.begin();

	std::cout << "------MAP------\n";
	for (mapIt curr = it; curr != map.end(); curr++) {
		std::cout << "Frist -> " << curr->first << " :: Second ->" << curr->second << '\n';
	}
	std::cout << "------MAP END------\n";
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
	this->url = splitReq[1];
	if (splitReq[1].find('?') != std::string::npos)
	{
		this->path = splitReq[1].substr(0, splitReq[1].find('?'));
		this->query = splitReq[1].substr(splitReq[1].find('?'), splitReq[1].length());
	} else {
		this->path = splitReq[1];
		this->query = "";
	}
	this->version = splitReq[2];
	buffer.erase(0, temp.length() + 1);
	if (buffer.empty())
		return ;
	this->header = setMap(buffer);
	this->host = getHeader("Host");
	std::cout << "get map: " << getHeader("Accept-Encoding") << '\n';
	if (buffer.empty())
		return ;
	buffer.erase(0, 3);
	this->body = setMap(buffer);
	// printMap(this->body);
}

//	SETTERS
void	Request::setCgiOutput(const std::string toSet) {
	this->_cgiOutput = toSet;
}

void Request::setClientId(const int clientId) {
	this->_clientId = clientId;
}


//	GETTERS
const std::string& Request::getMethod() const {
	return this->method;
}

const std::string& Request::getVersion() const {
	return this->version;
}

const std::string& Request::getPath() const {
	return this->path;
}

std::string Request::getUri() const {
	return "http://" + this->host + this->url;
}

const std::string& Request::getUrlPath() const {
	return this->url;
}

const std::string& Request::getHeader(const std::string& key) const {
	return this->header.at(key);
}

const std::string& Request::getBody(const std::string& key) const{
	return this->body.at(key);
}

const std::string& Request::getCgiOutput() const {
	return this->_cgiOutput;
}

const std::string& Request::getHost() const {
	return this->host;
}

const std::string& Request::getQuery() const {
	return this->query;
}

const int&	Request::getClientId() const {
	return this->_clientId;
}