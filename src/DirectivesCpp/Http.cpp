#include "../../include/Directives/Http.hpp"
#include "../../include/includes.hpp"
#include "../../include/Directives/Server.hpp"
#include "../../include/Directives/Location.hpp"
#include "../../include/Errors.hpp"
#include <iostream>
#include <sstream>
#include <string>

int serverN = 0;

Http::Http() : AConfig() {}

Http::~Http() {
	std::cout << RED << "\n{ Deleting http\n\n" << WHITE;
	cleanDirectives();
	std::cout << RED << "}\n" << WHITE;
}

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

