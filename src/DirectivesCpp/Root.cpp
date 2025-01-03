#include "../../include/Directives/Root.hpp"
#include "../../include/Errors.hpp"
#include "../../include/includes.hpp"
#include <vector>

// TODO: set real path when ready to test
Root::Root() : _path("/src/www/static") {}

Root::~Root() {}

const std::string&	Root::getPath() const {
	return this->_path;
}

void	Root::parseDirective(const std::vector<std::string>& args) {
	if (args.size() != 1) {
		throw Errors::TooFewArgsException("Wrong number of args", ConfigLine, __FILE__);
	}
	if (args[0].find(";") != std::string::npos) {
		if (*args[0].begin() != '/')
			throw Errors::InvalidPathException("Path must start with '/'", ConfigLine, __FILE__);
		this->_path = std::string(args[0].begin(), args[0].end() - 1);
		return;
	} else {
		throw Errors::NoSemiColonException("No semicolon found", ConfigLine, __FILE__);
	}
}