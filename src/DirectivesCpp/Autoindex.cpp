#include "../../include/Directives/Autoindex.hpp"
#include "../../include/Errors.hpp"
#include "../../include/includes.hpp"

Autoindex::Autoindex(const std::vector<std::string>& args) {
	if (args.size() != 1)
		throw Errors::TooFewArgsException();
	if (args[0] == "on")
		_autoindex = true;
	else if (args[0] == "off")
		_autoindex = false;
	else
		throw Errors::UnknownDirectiveException();
}

Autoindex::~Autoindex() {}

const bool&	Autoindex::getAutoindex() const {
	return _autoindex;
}