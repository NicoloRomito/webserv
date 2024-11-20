#include "../../include/includes.hpp"
#include "../../include/Directives/Listen.hpp"

Listen::Listen(std::vector<std::string> args) {
	if (args.size() < 1) {
		error("Listen directive must have 2 arguments");
		return;
	}
	_host = args[0];
	_port = args[1];
}

Listen::~Listen() {}