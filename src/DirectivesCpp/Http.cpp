#include "../../include/Directives/Http.hpp"
#include "../../include/Directives/Server.hpp"
#include <sstream>

Http::Http(std::stringstream& file) : AConfig() {
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
			if (directive == "server" || directive == "location") {
				_directives[directive] = createBlock(directive, file);
				continue;
			}
			_directives[directive] = createDirective(directive, args);
		}
	}
}

Http::~Http() {}

AConfig*	Http::createBlock(const std::string& directive, std::stringstream& file) {
	DirectiveType	type = getDirectiveType(directive);

	if (type == SERVER) {
		return new Server(file);
	} else if (type == LOCATION) {
		return new Location(file);
	} else {
		error("Unknown directive");
		return NULL;
	}
}