#include "../../include/Directives/CgiPass.hpp"


CgiPass::CgiPass(const std::vector<std::string>& args) {
	if (args.size() < 1) {
		error("CgiPass directive must have at least 1 argument");
		return;
	}
	_path = args[0];
}