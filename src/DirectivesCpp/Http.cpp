#include "../../include/Directives/Http.hpp"
#include "../../include/includes.hpp"
#include "../../include/Directives/Server.hpp"
#include "../../include/Directives/Location.hpp"
#include "../../include/Directives/Listen.hpp"
#include "../../include/Errors.hpp"
#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>

int	serverN = 0;

Http::Http() : AConfig(), _serverN(0) {}

Http::~Http() {
	cleanDirectives();
}

void	Http::compareServerPorts() {
	if (this->_serverN == 1)
		return;
	for (int i = 1; i < this->_serverN; i++) {

		std::string nextServer = "server" + int_to_string(i + 1);
		Server* serverToCheck = getDirective<Server>("server" + int_to_string(i));
		int serverNListen = serverToCheck->getNumberOfListen();
		int numberOfListenToCheck = getDirective<Server>(nextServer)->getNumberOfListen();

		if (serverNListen == 0 || numberOfListenToCheck == 0)
			continue;
		for (int j = 1; j <= serverNListen && serverNListen > 0; j++) {

			Listen* listenToCheck = serverToCheck->getDirective<Listen>("listen" + int_to_string(j));
			for (int k = 1; k <= numberOfListenToCheck; k++) {

				if (listenToCheck->getPort() == getDirective<Server>(nextServer)->getDirective<Listen>("listen" + int_to_string(k))->getPort()) {
					throw Errors::SameListenException("Error: Two servers cannot have the same port", ConfigLine, __FILE__);
				}
			}
		}
	}
}

// * This function is used to parse the configuration file.

void	Http::parse(std::stringstream& file) {
	std::string	line;
	std::string	directive;
	std::vector<std::string>	args;

	while (std::getline(file, line)) {
		ConfigLine++;
		if (line.empty() || !checkLine(line)) {
			continue;
		}
		if (line.find("}") == std::string::npos) { // if the closing bracket is still not found
			args = returnLine(line); // get the args divided in the line
			directive = parseDirective(args.at(0)); // get the name of the directive, if not returns UNKNOWN.
			if (directive == "server") {
				std::string serverName = directive + int_to_string(++this->_serverN);
				_directives[serverName] = createBlock(directive, file, "");
				continue;
			}
			args.erase(args.begin());
			if (directive == "client_max_body_size" && !alreadyExists("client_max_body_size") && semicolonFound(line))
				_directives[directive] = createDirective(directive, args);
		} else
			break;
		if (!line.empty())
			line.clear();
	}
	createDefaultDirectives(HTTP);
	args.clear();

	compareServerPorts();
}

std::string	Http::getServerName(const std::string& host) {
	std::string port = host.substr(host.find(":") + 1, (host.size() - host.find(":")) - 1);
	for (int i = 1; i < _serverN; i++) {
		std::string serverName = "server" + int_to_string(i);
		int numberOfListen = this->getDirective<Server>(serverName)->getNumberOfListen();
		for (int j = 1; j < numberOfListen; j++) {
			std::string listenName = "listen" + int_to_string(j);
			if (this->getDirective<Server>(serverName)->getDirective<Listen>(listenName)->getPort() == port)
				return "server" + int_to_string(i);
		}
	}
	return "";
}

std::string	Http::getLocationName(const std::string& path, const std::string& serverNumber) {
	int numberOfLocation = this->getDirective<Server>(serverNumber)->getNumberOfLocation();
	for (int j = 1; j < numberOfLocation; j++) {
		std::string locationName = serverNumber + "location" + int_to_string(j);
		if (this->getDirective<Server>(serverNumber)->getDirective<Location>(locationName)->getPath() == path)
			return locationName;
	}
	return "";
}

