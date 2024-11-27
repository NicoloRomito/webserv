#include "../../include/Directives/CgiPass.hpp"
#include "../../include/Errors.hpp"
#include "../../include/includes.hpp"

CgiPass::CgiPass(const std::vector<std::string>& args) {
	if (args.size() != 1) {
		throw Errors::TooFewArgsException("Wrong number of args", __LINE__, __FILE__);
	}
	_path = std::string(args[0].begin(), args[0].end() - 1);
}

CgiPass::~CgiPass() {}

const std::string&	CgiPass::getPath() const {
	return _path;
}