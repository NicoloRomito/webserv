#include "../../include/includes.hpp"
#include "../../include/Errors.hpp"
#include <vector>
#include "../../include/Directives/ServerName.hpp"

ServerName::ServerName(std::vector<std::string>& args) {
	if (args.size() < 1) {
		throw Errors::TooFewArgsException();
	}
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++) {
		this->_names.push_back(*it);
	}
}

ServerName::~ServerName() {
	this->_names.clear();
}

	// already checked if the args are empty //
const std::vector<std::string>&	ServerName::getNames() const {
	return this->_names;
}