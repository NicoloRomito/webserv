#include "../../include/includes.hpp"
#include "../../include/Errors.hpp"
#include <vector>
#include "../../include/Directives/ServerName.hpp"

ServerName::ServerName() {
	this->_names.push_back("localhost");
}

ServerName::~ServerName() {
	this->_names.clear();
}

	// already checked if the args are empty //
const std::vector<std::string>&	ServerName::getNames() const {
	return this->_names;
}

void	ServerName::parseDirective(const std::vector<std::string>& args) {
	if (args.size() < 1) {
		throw Errors::TooFewArgsException("Wrong number of args", ConfigLine, __FILE__);
	}
	for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); it++) {
		this->_names.push_back(std::string(it->begin(), it->end() - 1));
	}
}