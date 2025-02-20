#include "../../include/includes.hpp"
#include "../../include/Errors.hpp"
#include <vector>
#include "../../include/Directives/Index.hpp"

Index::Index() : _defaultFile("index.html") {}

Index::~Index() {}

const std::string&	Index::getFile() const {
	return this->_defaultFile;
}

void	Index::parseDirective(const std::vector<std::string>& args) {
	if (args.size() != 1) {
		throw Errors::TooFewArgsException("Wrong number of args", ConfigLine, __FILE__);
	}
	if (args[0].find(".") == std::string::npos)
	{
		throw Errors::UnknownFileException("Index must be a file", __FILE__);
	}
	_defaultFile = std::string(args[0].begin(), args[0].end() - 1); // remove the last character
}