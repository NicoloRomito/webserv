#include "../../include/includes.hpp"
#include "../../include/includeClasses.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <iostream>

bool	checkLine(const std::string& line) {
	for (size_t i = 0; i < line.size(); i++) {
		if (line[i] == '#') {
			return false;
		}
		if (line[i] == '\t' || line[i] == ' ' || line[i] == '\r') {
			continue;
		}
		else {
			return true;
		}
	}
	return false;
}

bool	isNumber(const std::string& str) {
	for (size_t i = 0; i < str.size(); i++) {
		if (!std::isdigit(str[i])) {
			return false;
		}
	}
	return true;
}

std::string int_to_string(int value) {
    std::ostringstream oss;
    oss << value;

	std::string str = oss.str();
	oss.clear();

    return str;
}

std::string to_string(char value) {
    std::ostringstream oss;
    oss << value - '0';

	std::string str = oss.str();
	oss.clear();

    return str;
}

bool	semicolonFound(const std::string& line) {
	size_t i = 0;
	for (i = line.size() - 1; i > 0; i--) {
		if (std::isspace(line[i])) {
			continue;
		} else break;
	}
	int count = 0;
	for (size_t j = i; j > 0; j--) {
		if (line[j] == ';') {
			count++;
		}
	}
	if (count != 1) {
		return false;
	}
	if (i == line.size()) {
		if (line[i - 1] == ';')
			return true;
	} else {
		if (line[i] == ';')
			return true;
	}
	return false;
}

bool	isADirectory(const std::string& urlPath, const std::string& root) {
	std::string newRoot;
	std::string requestPath;
	if (root.find(urlPath) != std::string::npos) {
		newRoot = root.substr(0, root.find(urlPath));
		requestPath = getCurrentDir() + newRoot + urlPath;
	} else 
		requestPath = getCurrentDir() + root + urlPath;
	std::cout << "REQUEST PATH: " << requestPath << std::endl;
	struct stat pathStat;
	if (stat(requestPath.c_str(), &pathStat) == 0) {
		if (S_ISDIR(pathStat.st_mode))
			return true;
	} else {
		std::cout << "Error: " << requestPath << " is not a directory\n";
	}
	return false;
}

bool	locationMatches(const std::string& urlPath, const std::string& locationPath) {
	if (urlPath == locationPath)
		return true;
	return false;
}

std::string	getCurrentDir() {
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return "";
	return std::string(cwd);
}

void	setAllValues(Response* res, Http* http, const std::string& serverName, const std::string& locationName, bool locationExists) {
	Server *server = http->getDirective<Server>(serverName);

	res->setClientMaxBodySize(http->getDirective<ClientMaxBodySize>("client_max_body_size")->getSize());
	res->setErrorPage4xx(server->getDirective<ErrorPage>(serverName + "error_page4xx")->getPath());
	res->setErrorPage5xx(server->getDirective<ErrorPage>(serverName + "error_page5xx")->getPath());
	res->setAvailableErrorCodes(server->getDirective<ErrorPage>(serverName + "error_page4xx")->getCodes(),
		server->getDirective<ErrorPage>(serverName + "error_page5xx")->getCodes());

	if (!locationExists) {
		if (server->getDirective<Index>("index"))
			res->setIndex(server->getDirective<Index>("index")->getFile());
		if (server->getDirective<Autoindex>("autoindex"))
			res->setAutoindex(server->getDirective<Autoindex>("autoindex")->getAutoindex());
		if (server->getDirective<Root>("root"))
			res->setRoot(server->getDirective<Root>("root")->getPath());
		if (server->getDirective<CgiPass>("cgi_pass"))
			res->setCgiPass(server->getDirective<CgiPass>("cgi_pass")->getPath());
		if (server->getDirective<ServerName>("server_name"))
			res->setServerNames(server->getDirective<ServerName>("server_name")->getNames());
		if (server->getDirective<ClientMaxBodySize>("client_max_body_size"))
			res->setClientMaxBodySize(server->getDirective<ClientMaxBodySize>("client_max_body_size")->getSize());
	} else {
		Location *location = server->getDirective<Location>(locationName);
		res->setLocationPath(location->getPath());
		// DEFAULT VALUES FOR LOCATION
		res->setIndex(location->getDirective<Index>("index")->getFile());
		res->setAutoindex(location->getDirective<Autoindex>("autoindex")->getAutoindex());
		res->setRoot(location->getDirective<Root>("root")->getPath());
		res->setCgiPass(location->getDirective<CgiPass>("cgi_pass")->getPath());
		// TODO search for directives if they exist
		if (location->getDirective<ClientMaxBodySize>("client_max_body_size"))
			res->setClientMaxBodySize(location->getDirective<ClientMaxBodySize>("client_max_body_size")->getSize());
		if (location->getDirective<ServerName>("server_name"))
			res->setServerNames(location->getDirective<ServerName>("server_name")->getNames());
		if (location->getDirective<ErrorPage>(serverName + "error_page4xx"))
			res->setErrorPage4xx(location->getDirective<ErrorPage>(serverName + "error_page4xx")->getPath());
		if (location->getDirective<ErrorPage>(serverName + "error_page5xx"))
			res->setErrorPage5xx(location->getDirective<ErrorPage>(serverName + "error_page5xx")->getPath());
		if (location->getDirective<ErrorPage>(serverName + "error_page4xx") || location->getDirective<ErrorPage>(serverName + "error_page5xx"))
			res->setAvailableErrorCodes(location->getDirective<ErrorPage>(serverName + "error_page4xx")->getCodes(),
				location->getDirective<ErrorPage>(serverName + "error_page5xx")->getCodes());
	}
	if (!res->getServerNames().empty())
		res->addServerNamesToHosts();
}