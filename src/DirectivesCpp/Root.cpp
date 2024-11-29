#include "../../include/Directives/Root.hpp"
#include "../../include/Errors.hpp"
#include "../../include/includes.hpp"

Root::Root() : _path("./") {}

Root::Root(const std::vector<std::string>& args) {
	if (args.size() != 1) {
		throw Errors::TooFewArgsException("Wrong number of args", __LINE__, __FILE__);
	}
	if (args[0].find(";") != std::string::npos) {
		this->_path = std::string(args[0].begin(), args[0].end() - 1);
		return;
	} else {
		throw Errors::NoSemiColonException("No semicolon found", __LINE__, __FILE__);
	}
}

Root::~Root() {}

const std::string&	Root::getPath() const {
	return this->_path;
}