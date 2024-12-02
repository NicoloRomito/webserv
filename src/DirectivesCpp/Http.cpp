#include "../../include/Directives/Http.hpp"
#include "../../include/includes.hpp"
#include "../../include/Directives/Server.hpp"
#include "../../include/Directives/Location.hpp"
#include "../../include/Errors.hpp"
#include <iostream>
#include <sstream>
#include <string>

int serverN = 0;

Http::Http(std::stringstream& file) : AConfig() {
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
			directive = parseDirective(args[0]); // get the name of the directive, if not returns UNKNOWN.
			if (directive.empty()) {
				continue;
			}
			if (directive == "server") {
				std::string serverName = directive + to_string(++serverN);
				_directives[serverName] = createBlock(directive, file);
				continue;
			}
			args.erase(args.begin());
			if (directive == "client_max_body_size" && !alreadyExists("client_max_body_size"))
				_directives[directive] = createDirective(directive, args);
		} else
			break;
		if (!line.empty())
			line.clear();
	}
	createDefaultDirectives(HTTP);
	args.clear();
}

Http::~Http() {
	std::map<std::string, AConfig*>::iterator it = this->_directives.begin();
	for (; it != this->_directives.end(); it++) {
		std::cout << "deleting " << it->first << std::endl;
		if (it->second)
			delete it->second;
	}
	this->_directives.clear();
}

AConfig*	Http::createBlock(const std::string& directive, std::stringstream& file) {
	DirectiveType	type = getDirectiveType(directive);

	if (type == SERVER) {
		return new Server(file);
	} else if (type == LOCATION) {
		return new Location(file);
	} else {
		throw Errors::UnknownDirectiveException("Unknown directive", ConfigLine, __FILE__);
	}
}
