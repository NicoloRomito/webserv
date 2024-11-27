#include "../../include/Directives/Location.hpp"

Location::Location(std::stringstream& file) : AConfig() {
	std::string	line;
	std::string	directive;
	std::vector<std::string>	args;

	while (std::getline(file, line)) {
		if (line.empty() || !checkLine(line)) {
			continue;
		}
		if (line.find("{") != std::string::npos) {
			_path = returnLine(line)[1];
			continue;
		}
		if (line.find("}") == std::string::npos) { // if the closing bracket is still not found
			args = returnLine(line); // get the args divided in the line
			directive = parseDirective(args[0]); // get the name of the directive, if not returns UNKNOWN.
			if (directive.empty()) {
				continue;
			}
			args.erase(args.begin());
			_directives[directive] = createDirective(directive, args);
		} else
			break;
	}
	createDefaultDirectives(LOCATION);
}

Location::~Location() {
	std::map<std::string, AConfig*>::iterator it = this->_directives.begin();
	for (; it != this->_directives.end(); it++) {
		delete it->second;
	}
	this->_directives.clear();
}

const std::string&	Location::getPath() const {
	return _path;
}