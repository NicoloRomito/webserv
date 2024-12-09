#include "../../include/Directives/CgiPass.hpp"
#include "../../include/Errors.hpp"
#include "../../include/includes.hpp"
#include <vector>

CgiPass::CgiPass() {}

CgiPass::~CgiPass() {}

// TODO: set real paths when ready
void	CgiPass::setDefaultCgiPaths() {
	_cgiPaths.push_back("src/cgi-bin/php-cgi");
	_cgiPaths.push_back("src/cgi-bin/python-cgi");
	_cgiPaths.push_back("src/cgi-bin/bash-cgi");
}

const std::vector<std::string>&	CgiPass::getPath() const {
	return _cgiPaths;
}

void	CgiPass::parseDirective(const std::vector<std::string>& args) {
	if (args.size() < 1 || args.size() > 3) {
		throw Errors::TooFewArgsException("Wrong number of args", ConfigLine, __FILE__);
	}
	// cgi paths can be 3 at most
	for (std::vector<std::string>::iterator it = _cgiPaths.begin(); it != _cgiPaths.end(); it++) {
		if (it == _cgiPaths.end() - 1) {
			_cgiPaths.push_back(std::string(args[args.size()].begin(), args[args.size()].end() - 1)); // remove the last character
		} else
			_cgiPaths.push_back(*it);
	}
}