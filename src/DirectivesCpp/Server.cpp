#include "../../include/Directives/Server.hpp"
#include "../../include/Directives/Location.hpp"

Server::Server(std::stringstream& file) : AConfig() {
	std::string	line;
	std::string	directive;
	std::vector<std::string>	args;

	while (std::getline(file, line)) {
		if (line.empty()) {
			continue;
		}
		if (line.find("}") == std::string::npos) { // if the closing bracket is still not found
			args = returnLine(line); // get the args divided in the line
			directive = parseDirective(args[0]); // get the name of the directive, if not returns UNKNOWN.
			if (directive.empty()) {
				continue;
			}
            if (directive == "location") {
				_directives[directive] = createBlock(directive, file);
				continue;
			}
			_directives[directive] = createDirective(directive, args);
		} else
			break;
	}
}

Server::~Server() {
	std::map<std::string, AConfig*>::iterator it = this->_directives.begin();
	for (; it != this->_directives.end(); it++) {
		delete it->second;
	}
	this->_directives.clear();
}

AConfig*	Server::createBlock(const std::string& directive, std::stringstream& file) {
	DirectiveType	type = getDirectiveType(directive);

	if (type == LOCATION) {
		return new Location(file);
	} else {
		error("Unknown directive");
		return NULL;
	}
}