#include "../../../include/Directives/config.hpp"
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string>	returnLine(const std::string& line);

AConfig::AConfig(std::string file)
{
	if (file.find(".conf") == std::string::npos) {
		error("File type not correct");
	}

	std::ifstream	configFile(file);
	std::string		token, directive, args;

	if (!configFile) {
		error("File opening failed");
	}

	std::vector<std::string>::iterator it = _tokens.begin();

	while (std::getline(configFile, token)) {
		_tokens = returnLine(token); // get the args divided in the line
		directive = parseDirective(_tokens[0]); // get the name of the directive, if not returns UNKNOWN.

		_directives[directive] = createDirective(directive, _tokens); // create the new directive

	} // how do I pop the first element of the array(directive?),the array will be passed as argument for construction to the new directive.


}

AConfig::~AConfig() {}

std::vector<std::string>	returnLine(const std::string& line) {
	std::string	token;
	std::vector<std::string>	tokens;
	std::istringstream	fileLine(line);

	while (fileLine >> token) {
		tokens.push_back(token);
	}

	return tokens;
}


AConfig*	AConfig::createDirective(const std::string& directive, std::vector<std::string> args) {
	DirectiveType	type = getDirectiveType(directive);

	switch (type) {
		case HTTP:
			return new Http();
		case LISTEN:
			return new Listen(args);
		case SERVER_NAME:
			return new ServerName();
		case ROOT:
			return new Root();
		case INDEX:
			return new Index();
		case ERROR_PAGE:
			return new ErrorPage();
		case CLIENT_MAX_BODY_SIZE:
			return new ClientMaxBodySize();
		case AUTOINDEX:
			return new Autoindex();
		case LOCATION:
			return new Location();
		case CGI_PASS:
			return new CgiPass();
		case SERVER:
			return new Server();
		default:
			error("Unknown directive");
	}
	return NULL;
}


std::string	parseDirective(const std::string& directive)
{
	DirectiveType	type = getDirectiveType(directive);

	if (type == UNKNOWN) {
		error("Unknown directive");
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

