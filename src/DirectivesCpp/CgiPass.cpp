#include "../../include/Directives/CgiPass.hpp"
#include "../../include/Errors.hpp"
#include "../../include/includes.hpp"

CgiPass::CgiPass(const std::vector<std::string>& args) {
	if (args.size() != 1) {
		throw Errors::TooFewArgsException();
	}
	_path = args[0];
}

CgiPass::~CgiPass() {}

const std::string&	CgiPass::getPath() const {
	return _path;
}