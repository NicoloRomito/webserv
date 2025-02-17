#include "../../include/includeClasses.hpp" // IWYU pragma: keep
#include "../../include/Errors.hpp"
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <math.h>
#include <string>
#include <vector>

int	locationN = 0;

Server::Server() : AConfig(), _nListen(0), _locationN(0) {}

Server::~Server() {
	if (this->getDirective<ServerName>("server_name") != NULL)
		removeServerNamesFromHosts();
	cleanDirectives();
}

void	Server::parse(std::stringstream& file) {
	std::string	line;
	std::string	directive;
	std::vector<std::string>	args;

	this->_dName = "server" + int_to_string(++serverN);

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
				std::string	locationName = this->_dName + directive + int_to_string(++_locationN);
				_directives[locationName] = createBlock(directive, file, args[1]);
				continue;
			}
			args.erase(args.begin());
			if (semicolonFound(line)) {
				if (directive == "error_page") {
					if (!parseErrorPage(args, directive))
						throw Errors::InvalidErrorCode("Invalid error code", ConfigLine, __FILE__);
				} else if (directive == "listen") {
					parseListen(args, directive, ++_nListen);
				} else if (directive != "server" && directive != "http" && !alreadyExists(directive))
					_directives[directive] = createDirective(directive, args);
			} else
				throw Errors::NoSemiColonException("Semicolon Exception: Too many semicolons or no semicolon found", ConfigLine, __FILE__);
		} else
			break;
	}
	createDefaultDirectives(SERVER);
	args.clear();
}

void	Server::parseListen(const std::vector<std::string>& args, const std::string& directive, int listenN) {
	std::string	listenName = directive + int_to_string(listenN);
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
		std::set<int>	existingCodes = this->getDirective<ErrorPage>(errorPageName)->getCodes();
		std::vector<std::string>::const_iterator argsIt = args.begin();
		for (std::set<int>::iterator it = existingCodes.begin(); it != existingCodes.end() && argsIt != args.end(); it++, argsIt++) {
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

void	Server::addServerNamesToHosts()
{
	std::ifstream	fileIn("/etc/hosts");
	if (!fileIn.is_open()) {
		printError("Error opening /etc/hosts for reading");
		return;
	}

	std::vector<std::string>	fileLines;
	std::string					line;
	while (std::getline(fileIn, line)) {
		fileLines.push_back(line);
	}
	fileIn.close();

	std::set<std::string>	_serverNames = this->getDirective<ServerName>("server_name")->getNames();

	for (std::set<std::string>::iterator Serverit = _serverNames.begin(); Serverit != _serverNames.end(); ++Serverit) {
		std::string serverName = "127.0.0.1 " + *Serverit;
		for (std::vector<std::string>::iterator it = fileLines.begin(); it != fileLines.end(); ++it) {
			if (it->find(serverName) != std::string::npos) {
				return;
			}
		}
		fileLines.push_back(serverName);
	}

	std::ofstream	fileOut("/etc/hosts");
	if (!fileOut.is_open()) {
		printError("Error opening /etc/hosts for writing. Use sudo when running the server.");
		return;
	}

	for (std::vector<std::string>::iterator it = fileLines.begin(); it != fileLines.end(); ++it) {
		fileOut << *it << std::endl;
	}

	fileOut.close();
}


void	Server::removeServerNamesFromHosts() {
	std::ifstream	fileIn("/etc/hosts");
	if (!fileIn.is_open()) {
		printError("Error opening /etc/hosts for reading");
		return;
	}

	std::vector<std::string>	fileLines;
	std::string					line;
	while (std::getline(fileIn, line))
		fileLines.push_back(line);
	fileIn.close();

	std::set<std::string>	_serverNames = this->getDirective<ServerName>("server_name")->getNames();

	for (std::vector<std::string>::iterator i = fileLines.end() - 1; i != fileLines.begin(); i--) {
		for (std::set<std::string>::iterator it = _serverNames.begin(); it != _serverNames.end(); ++it) {
			if (i->find("127.0.0.1 " + *it) != std::string::npos)
				fileLines.erase(i);
		}
	}

	std::ofstream	fileOut;
	fileOut.open("/etc/hosts");
	if (!fileOut.is_open()) {
		printError("Error opening /etc/hosts for writing. Use sudo when running the server.");
		return;
	}

	for (std::vector<std::string>::iterator it = fileLines.begin(); it != fileLines.end(); ++it) {
		fileOut << *it << std::endl;
	}

	fileOut.close();
}

int	Server::getNumberOfListen() const {
	return _nListen;
}

int	Server::getNumberOfLocation() const {
	return this->_locationN;
}