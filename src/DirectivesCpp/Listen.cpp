#include "../../include/includes.hpp"
#include <cctype>
#include <string>
#include "../../include/Directives/Listen.hpp"

bool	isNumber(const std::string& str) {
	for (size_t i = 0; i < str.size(); i++) {
		if (!std::isdigit(str[i])) {
			return false;
		}
	}
	return true;
}

Listen::Listen(const std::vector<std::string>& args) {
	if (args.size() < 1) {
		error("Listen directive must have at least 1 argument");
		return;
	}
	if (args[0].find(":") != std::string::npos) {
		if (!isNumber(args[0].substr(args[0].find(":") + 1)))
			throw std::invalid_argument("Port must be a number");
		_port = args[0].substr(args[0].find(":") + 1);
		_ip = args[0].substr(0, args[0].find(":"));
	}
	else {
		if (!isNumber(args[0]))
			throw std::invalid_argument("Port must be a number");
		_port = args[0];
		_ip = "localhost";
	}
}

Listen::~Listen() {}