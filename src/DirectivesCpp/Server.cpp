#include "../../include/Directives/Server.hpp"
#include "../../include/Directives/ErrorPage.hpp"
#include "../../include/Directives/Location.hpp"
#include "../../include/Errors.hpp"
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <math.h>
#include <string>
#include <vector>

int	locationN = 0;
int listenN = 0;

Server::Server() : AConfig() {}

Server::~Server() {
	// std::cout << GREEN << "\n{ Deleting server\n\n" << WHITE;
	cleanDirectives();
	// std::cout << GREEN << "}\n" << WHITE;
}

void	Server::parse(std::stringstream& file) {
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
			if (semicolonFound(line)) {
				if (directive == "error_page") {
					if (!parseErrorPage(args, directive))
						throw Errors::InvalidErrorCode("Invalid error code", ConfigLine, __FILE__);
					// TODO : check if error page already exists and if the status code is different the directive before it already created.
					// TODO : if the status code is different but the directive already exists, add the status code to the directive already created.
				} else if (directive == "listen") {
					parseListen(args, directive, ++listenN);
				} else if (directive != "server" && directive != "http" && !alreadyExists(directive))
					_directives[directive] = createDirective(directive, args);
			} else
				throw Errors::NoSemiColonException("Semicolon at the end of line not found", ConfigLine, __FILE__);
		} else
			break;
	}
	createDefaultDirectives(SERVER);
	args.clear();
}

void	Server::parseListen(const std::vector<std::string>& args, const std::string& directive, int listenN) {
	std::string	listenName = directive + to_string(listenN);
	if (!alreadyExists(listenName)) {
		_directives[listenName] = createDirective(directive, args);
	} else
		throw Errors::DuplicateDirectiveException("Duplicate directives are not allowed", ConfigLine, __FILE__);
}

bool	Server::parseErrorPage(const std::vector<std::string>& args, const std::string& directive) {
	std::string errorPageName = this->_dName + directive + to_string(args[0].at(0)) + "xx";
	char	codeType = args.begin()->at(0);
	if ((codeType == '5' || codeType == '4') && !alreadyExists(errorPageName)) {
			_directives[errorPageName] = createDirective(directive, args);
		return true;
	} else if (alreadyExists(errorPageName) && args.begin()->size() == 3) {
		std::vector<int>	existingCodes = this->getDirective<ErrorPage>(errorPageName)->getCodes();
		std::vector<std::string>::const_iterator argsIt = args.begin();
		for (std::vector<int>::iterator it = existingCodes.begin(); it != existingCodes.end() && argsIt != args.end(); it++, argsIt++) {
			if (argsIt->size() != 3) {
				return false;
			}
			if (to_string(*it).at(0) != codeType || this->getDirective<ErrorPage>(errorPageName)->getPath() != *(args.end() - 1)) { 	// if the error code is different (4xx or 5xx) from the existing one
				std::cout << "Error: " << errorPageName << " already exists\n";
				return false;
			}
			if (isNumber(*argsIt) && argsIt->at(0) == codeType && *it == atoi(argsIt->c_str())) { // if the error code already exists
				continue;
			} else if (isNumber(*argsIt) && argsIt->at(0) == codeType && *it != atoi(argsIt->c_str()) && argsIt->size() == 3) { // if the error code is different
				this->getDirective<ErrorPage>(errorPageName)->addErrorCode(*argsIt);
			}
		}
		return true;
	}

	return false;
}
