#include "../../include/Directives/Server.hpp"
#include "../../include/Directives/Location.hpp"
#include "../../include/Errors.hpp"
#include <iostream>
#include <stdint.h>
#include <math.h>
#include <string>

int	locationN = 0;

Server::Server(std::stringstream& file) : AConfig() {
	std::string	line;
	std::string	directive;
	std::vector<std::string>	args;

	this->_dName = "server" + to_string(serverN);

	while (std::getline(file, line)) {
		ConfigLine++;
		if (line.empty() || !checkLine(line)) {
			continue;
		}
		if (line.find("}") == std::string::npos) { // if the closing bracket is still not found
			args = returnLine(line); // get the args divided in the line
			directive = parseDirective(args[0]); // get the name of the directive, if not returns UNKNOWN.
			if (directive.empty()) {
				continue;
			}
            if (directive == "location") {
				std::string	locationName = directive + to_string(++locationN);
				_directives[this->_dName + locationName] = createBlock(directive, file);
				continue;
			}
			args.erase(args.begin());
			if (directive == "error_page") {
				std::string errorPageName = directive + to_string(args[0][0]) + "xx";
				// TODO : check if error page already exists and if the status code is different the directive before it already created.
				// TODO : if the status code is different but the directive already exists, add the status code to the directive already created.
				if ((args[0].at(0) == '4' || args[0].at(0) == '5') && !alreadyExists(this->_dName + errorPageName))
					_directives[this->_dName + errorPageName] = createDirective(directive, args);
			}
			else if (directive != "server" && directive != "http" && !alreadyExists(directive))
				_directives[directive] = createDirective(directive, args);
		} else
			break;
	}
	createDefaultDirectives(SERVER);
	args.clear();
}

Server::~Server() {
	std::map<std::string, AConfig*>::iterator it = this->_directives.begin();
	for (; it != this->_directives.end(); it++) {
		std::cout << "deleting " << it->first << std::endl;
		if (it->second)
			delete it->second;
	}
	this->_directives.clear();
}

AConfig*	Server::createBlock(const std::string& directive, std::stringstream& file) {
	DirectiveType	type = getDirectiveType(directive);

	if (type == LOCATION) {
		return new Location(file);
	} else {
		throw Errors::UnknownDirectiveException("Unknown directive", ConfigLine, __FILE__);
	}
}
