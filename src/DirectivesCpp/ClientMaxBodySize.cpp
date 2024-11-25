#include "../../include/includes.hpp"
#include "../../include/Errors.hpp"
#include <cstdlib>
#include "../../include/Directives/ClientMaxBodySize.hpp"

ClientMaxBodySize::ClientMaxBodySize(const std::vector<std::string>& args) {
	if (args.size() != 1)
		throw Errors::TooFewArgsException();
	if (!isNumber(args[0]))
		throw std::invalid_argument("Invalid body size\n");
	_bodySize = atoll(args[0].c_str());
}

ClientMaxBodySize::~ClientMaxBodySize() {}

const size_t&	ClientMaxBodySize::getSize() const {
	return _bodySize;
}