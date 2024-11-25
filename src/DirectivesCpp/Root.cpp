#include "../../include/Directives/Root.hpp"
#include "../../include/Errors.hpp"
#include "../../include/includes.hpp"

Root::Root(const std::vector<std::string>& args) {
	if (args.size() != 1) {
		throw Errors::TooFewArgsException();
	}
	_path = args[0];
}

Root::~Root() {}

const std::string&	Root::getPath() const {
	return this->_path;
}