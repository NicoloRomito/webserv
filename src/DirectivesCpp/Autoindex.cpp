#include "../../include/Directives/Autoindex.hpp"
#include "../../include/Errors.hpp"
#include "../../include/includes.hpp"

Autoindex::Autoindex() : _autoindex(false) {}

Autoindex::~Autoindex() {}

const bool&	Autoindex::getAutoindex() const {
	return _autoindex;
}

void	Autoindex::parseDirective(const std::vector<std::string>& args) {
	if (args.size() != 1)
		throw Errors::TooFewArgsException("Wrong number of args", ConfigLine, __FILE__);
	if (std::string(args[0].begin(), args[0].end() - 1) == "on")
		_autoindex = true;
	else if (std::string(args[0].begin(), args[0].end() - 1) == "off")
		_autoindex = false;
	else
		throw Errors::UnknownDirectiveException("Unknown directive", ConfigLine, __FILE__);
}