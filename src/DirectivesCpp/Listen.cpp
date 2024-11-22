#include "../../include/includes.hpp"
#include <cctype>
#include "../../include/Directives/Listen.hpp"

Listen::Listen(const std::vector<std::string>& args) {
	if (args.size() < 1) {
		error("Listen directive must have at least 1 argument");
		return;
	}
	if (std::isdigit(args[0], std::locale())) {
		_host = "
	_host = args[0];
	_port = args[1];
}

Listen::~Listen() {}