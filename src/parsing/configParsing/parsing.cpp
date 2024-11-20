#include "../../../include/Directives/Http.hpp"
#include "../../../include/Directives/Server.hpp"
#include "../../../include/Directives/Location.hpp"
#include "../../../include/includes.hpp"
#include <algorithm>
#include <cstddef>
#include <sstream>
#include <string>

std::map<std::string, AConfig*>	_directives;

AConfig*	createBlock(const std::string& directive, std::stringstream file) {
	DirectiveType	type = getDirectiveType(directive);

	if (type == HTTP) {
		return new Http(file);
	} else if (type == SERVER) {
		return new Server(file);
	} else if (type == LOCATION) {
		return new Location(file);
	} else {
		error("Unknown directive");
		return NULL;
	}
}

std::string	parseDirective(const std::string& directive)
{
	DirectiveType	type = getDirectiveType(directive);

	if (type == UNKNOWN) {
		error("Unknown directive");
		return NULL;
	}
	return directive;
}

DirectiveType	getDirectiveType(const std::string& dir) {
		std::map<std::string, DirectiveType>	model = {
		{"listen", LISTEN},
		{"server_name", SERVER_NAME},
		{"root", ROOT},
		{"index", INDEX},
		{"error_page", ERROR_PAGE},
		{"client_max_body_size", CLIENT_MAX_BODY_SIZE},
		{"autoindex", AUTOINDEX},
		{"location", LOCATION},
		{"cgi_pass", CGI_PASS},
		{"server", SERVER},
		{"http", HTTP}
	};

	std::map<std::string, DirectiveType>::iterator it = model.find(dir);
	if (it == model.end()) {
		return UNKNOWN;
	}
	return it->second;
}


std::vector<std::string>	returnLine(const std::string& line) {
	std::string	token;
	std::vector<std::string>	tokens;
	std::istringstream	fileLine(line);

	while (fileLine >> token) {
		tokens.push_back(token);
	}

	return tokens;
}

void	startParsing(std::string file) {
	if (file.find(".conf") == std::string::npos) {
		error("File type not correct");
	}

	std::ifstream	configFile(file);
	std::stringstream	configStream;
	configStream << configFile.rdbuf();
	std::string		token, directive, args;

	if (!configFile) {
		error("File opening failed");
	}

	std::vector<std::string>	_tokens;

	if (std::getline(configFile, token)) {
		if (token != "http {") {
			error("First directive must be http");
			return;
		}
	}

	while (std::getline(configFile, token)) {
		_tokens = returnLine(token); // get the args divided in the line
		directive = parseDirective(_tokens[0]); // get the name of the directive, if not returns UNKNOWN.
		if (directive == "http" && std::find(_tokens.begin(), _tokens.end(), "{") != _tokens.end()) {
			_tokens.erase(_tokens.begin());
			_directives[directive] = createDirective(directive, _tokens, configStream); // create the new directive
		}
		else if (directive == "server" && std::find(_tokens.begin(), _tokens.end(), "{") != _tokens.end()) {
			_tokens.erase(_tokens.begin());
			_directives[directive] = createDirective(directive, _tokens); // create the new directive
		}
		else if (directive == "location" && std::find(_tokens.begin(), _tokens.end(), "{") != _tokens.end()) {
			_tokens.erase(_tokens.begin());
			_directives[directive] = createDirective(directive, _tokens); // create the new directive
		}
		_tokens.erase(_tokens.begin());
		_directives[directive] = createDirective(directive, _tokens); // create the new directive
	}
}