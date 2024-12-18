#include "../../include/Directives/Location.hpp"
#include "../../include/Directives/Server.hpp"
#include "../../include/Directives/Http.hpp"
#include "../../include/includes.hpp"
#include "../../include/Errors.hpp"

Location::Location() : AConfig() {}

Location::~Location() {
	// std::cout << "\n{ Deleting location\n\n";
	cleanDirectives();
	// std::cout << "}\n";
}

void	Location::parse(std::stringstream& file) {
	std::string	line;
	std::string	directive;
	std::vector<std::string>	args;

	this->_dName = "location" + int_to_string(locationN);

	while (std::getline(file, line)) {
		ConfigLine++;
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
			args.erase(args.begin());
			if (semicolonFound(line)) {
				if (directive == "error_page") {
					std::string errorPageName = directive + args.begin()->at(0) + "xx";
					_directives[errorPageName] = createDirective(directive, args);
				} else if (directive != "server" && directive != "http" && directive != "location" && !alreadyExists(directive))
					_directives[directive] = createDirective(directive, args);
			} else
				throw Errors::NoSemiColonException("Semicolon at the end of line not found", ConfigLine, __FILE__);
		} else
			break;
	}
	createDefaultDirectives(LOCATION);
	args.clear();
}

const std::string&	Location::getPath() const {
	return _path;
}

const AConfig*	Location::getDirective(const std::string& directiveName) const {
	std::map<std::string, AConfig*>::const_iterator it = this->_directives.find(directiveName);

	if (it != this->_directives.end()) {
		return it->second;
	}
	throw Errors::UnknownDirectiveException("Unknown directive", ConfigLine, __FILE__);
}