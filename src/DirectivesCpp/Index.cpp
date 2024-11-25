#include "../../include/includes.hpp"
#include "../../include/Errors.hpp"
#include "../../include/Directives/Index.hpp"

Index::Index(const std::vector<std::string>& args) {
	if (args.size() != 1) {
		throw Errors::TooFewArgsException();
	}
	_defaultFile = args[0];
}

Index::~Index() {}

const std::string&	Index::getPath() const {
	return this->_defaultFile;
}