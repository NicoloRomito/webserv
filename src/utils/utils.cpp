#include "../../include/includes.hpp"
#include <cctype>
#include <cstddef>

bool	checkLine(const std::string& line) {
	for (size_t i = 0; i < line.size(); i++) {
		if (line[i] == '#') {
			return false;
		}
		if (line[i] == '\t' || line[i] == ' ') {
			continue;
		}
		else {
			return true;
		}

	}
	return false;
}

bool	isNumber(const std::string& str) {
	for (size_t i = 0; i < str.size(); i++) {
		if (!std::isdigit(str[i])) {
			return false;
		}
	}
	return true;
}

std::string to_string(int value) {
    std::ostringstream oss;
    oss << value;
	if (oss.str().empty())
		return "";
    return oss.str();
}

DirectiveType	checkDirectiveType(const std::string& dir) {

		std::map<std::string, DirectiveType>	model;
		
		for (int i = 1; i < locationN; i++) {
			model["location" + to_string(i)] = LOCATION;
		}
		for (int i = 1; i < serverN; i++) {
			model["server" + to_string(i)] = SERVER;
		}
		model["http"] = HTTP;
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