#include "../../include/includes.hpp"
#include "../../include/Errors.hpp"
#include "../../include/Directives/Index.hpp"

Index::Index() : _defaultFile("index.html") {}

Index::Index(const std::vector<std::string>& args) {
	if (args.size() != 1) {
		throw Errors::TooFewArgsException("Wrong number of args", __LINE__, __FILE__);
	}
	_defaultFile = std::string(args[0].begin(), args[0].end() - 1); // remove the last character
}

Index::~Index() {}

const std::string&	Index::getPath() const {
	return this->_defaultFile;
}