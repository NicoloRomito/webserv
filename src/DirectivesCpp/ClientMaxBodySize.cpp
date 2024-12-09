#include "../../include/includes.hpp"
#include "../../include/Errors.hpp"
#include <cctype>
#include <climits>
#include <cstddef>
#include <cstdlib>
#include "../../include/Directives/ClientMaxBodySize.hpp"

ClientMaxBodySize::ClientMaxBodySize() : _bodySize(100000000) {}

ClientMaxBodySize::~ClientMaxBodySize() {}

const size_t&	ClientMaxBodySize::getSize() const {
	return _bodySize;
}

void	ClientMaxBodySize::parseDirective(const std::vector<std::string>& args) {
	if (args.size() != 1)
		throw Errors::TooFewArgsException("Wrong number of args", ConfigLine, __FILE__);
	if (!isNumber(std::string(args[0].begin(), args[0].end() - 1)) && args[0].find("m")) {
		for (size_t i = 0; i < args[0].size() - 1; i++) {
			if ((!isdigit(args[0][i]) && args[0][i] != 'm') || !isdigit(args[0][0]))
				throw Errors::InvalidBodySizeException("Invalid body size", ConfigLine, __FILE__);
		}
		if (args[0][0] * 100000000 > INT_MAX)
			throw Errors::InvalidBodySizeException("Invalid body size", ConfigLine, __FILE__);
		_bodySize = args[0][0] * 100000000;
	} else if (!isNumber(std::string(args[0].begin(), args[0].end() - 1)))
		throw Errors::InvalidBodySizeException("Invalid body size", ConfigLine, __FILE__);
	
	_bodySize = atoll(std::string(args[0].begin(), args[0].end() - 1).c_str());
}