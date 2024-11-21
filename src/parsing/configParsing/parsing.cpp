#include "../../../include/Directives/Http.hpp"
#include "../../../include/Directives/Server.hpp"
#include "../../../include/Directives/Location.hpp"
#include "../../../include/includes.hpp"
#include <algorithm>
#include <cstddef>
#include <sstream>
#include <string>

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

	while (std::getline(configFile, token)) {
		if (token.find("http") != std::string::npos) {
			configStream << configFile.rdbuf();
			Http*	http = new Http(configStream);
			return;
		}
	}
	error("Configuration File must contain Http block");
}