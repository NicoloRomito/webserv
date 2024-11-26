#include "../../../include/Directives/Http.hpp"
#include "../../../include/Directives/Server.hpp"
#include "../../../include/Directives/Location.hpp"
#include "../../../include/includes.hpp"
#include "../../../include/Errors.hpp"
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <string>

std::string	parseDirective(const std::string& directive)
{
	DirectiveType	type = getDirectiveType(directive);

	if (type == UNKNOWN) {
		throw Errors::UnknownDirectiveException("Unknown directive " + directive, __LINE__, __FILE__);
	}
	return directive;
}

DirectiveType	getDirectiveType(const std::string& dir) {
		std::map<std::string, DirectiveType>	model;

		model["server"] = SERVER;
		model["http"] = HTTP;
		model["location"] = LOCATION;
		model["cgi_pass"] = CGI_PASS;
		model["autoindex"] = AUTOINDEX;
		model["client_max_body_size"] = CLIENT_MAX_BODY_SIZE;
		model["error_page"] = ERROR_PAGE;
		model["index"] = INDEX;
		model["root"] = ROOT;
		model["server_name"] = SERVER_NAME;
		model["listen"] = LISTEN;

	std::map<std::string, DirectiveType>::iterator it = model.find(dir);
	if (it != model.end()) {
		return it->second;
	}
	return UNKNOWN;
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

void	startParsing(const std::string& file) {
	if (file.find(".conf") == std::string::npos) {
		Errors::error("File type not correct", __LINE__, __FILE__);
		return;
	}

	std::ifstream	configFile(file.c_str());
	std::stringstream	configStream;
	std::string		token, directive, args;

	if (!configFile) {
		Errors::error("File opening failed", __LINE__, __FILE__);
		return;
	}

	while (std::getline(configFile, token)) {
		if (token.find("http") != std::string::npos) {
			configStream << configFile.rdbuf();
			Http*	http = new Http(configStream);

			(void)http;
			return;
		}
	}
	Errors::error("Configuration File must contain Http block", __LINE__, __FILE__);
}