#include "../../include/Directives/CgiPass.hpp"
#include "../../include/Errors.hpp"
#include "../../include/includes.hpp"
#include <vector>

CgiPass::CgiPass() {}

CgiPass::CgiPass(const std::vector<std::string>& args) {
	if (args.size() < 1 || args.size() > 3) {
		throw Errors::TooFewArgsException("Wrong number of args", ConfigLine, __FILE__);
	}
	for (std::vector<std::string>::iterator it = _cgiPaths.begin(); it != _cgiPaths.end(); it++) {
		if (it == _cgiPaths.end() - 1) {
			_cgiPaths.push_back(std::string(args[args.size()].begin(), args[args.size()].end() - 1));
		} else
			_cgiPaths.push_back(*it);
	}
}

CgiPass::~CgiPass() {}

const std::vector<std::string>&	CgiPass::getPath() const {
	return _cgiPaths;
}