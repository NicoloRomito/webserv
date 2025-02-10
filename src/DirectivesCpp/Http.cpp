#include "../../include/Directives/Http.hpp"
#include "../../include/includes.hpp"
#include "../../include/Directives/Server.hpp"
#include "../../include/Directives/Location.hpp"
#include "../../include/Directives/Listen.hpp"
#include "../../include/Errors.hpp"
#include <cstddef>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

int	serverN = 0;

Http::Http() : AConfig(), _serverN(0) {}

Http::~Http() {
	cleanDirectives();
}

void	Http::compareServerPorts() {
	std::set<std::string>	ports;
	for (int i = 0; i < this->_serverN; i++) {
		std::string serverName = "server" + int_to_string(i + 1);
		int numberOfListen = this->getDirective<Server>(serverName)->getNumberOfListen();
		for (int j = 0; j < numberOfListen; j++) {
			std::string listenName = "listen" + int_to_string(j + 1);
			std::string port = this->getDirective<Server>(serverName)->getDirective<Listen>(listenName)->getPort();
			if (ports.find(port) != ports.end())
				throw Errors::SameListenException("Duplicate port not allowed", __FILE__);
			ports.insert(port);
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
	for (int i = 0; i < _serverN; i++) {
		std::string serverName = "server" + int_to_string(i + 1);
		int numberOfListen = this->getDirective<Server>(serverName)->getNumberOfListen();
		for (int j = 0; j < numberOfListen; j++) {
			std::string listenName = "listen" + int_to_string(j + 1);
			if (this->getDirective<Server>(serverName)->getDirective<Listen>(listenName)->getPort() == port)
				return "server" + int_to_string(i + 1);
		}
	}
	return ""; //ho provato a mettere server1 come ritorno e telnet funzionava -Bard
}

std::string	Http::getLocationName(const std::string& path, const std::string& serverNumber) {

	int numberOfLocation = 0;
	if (this->getDirective<Server>(serverNumber))
		numberOfLocation = this->getDirective<Server>(serverNumber)->getNumberOfLocation();
	
	for (int j = 0; j < numberOfLocation; j++) {
		std::string locationName = serverNumber + "location" + int_to_string(j + 1);
		if (this->getDirective<Server>(serverNumber)->getDirective<Location>(locationName)->getPath() == path)
			return locationName;
	}
	return "";
}

int	Http::getServerN() const {
	return this->_serverN;
}

