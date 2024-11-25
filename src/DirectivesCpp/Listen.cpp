#include "../../include/includes.hpp"
#include "../../include/Directives/Listen.hpp"
#include "../../include/Errors.hpp"

Listen::Listen(const std::vector<std::string>& args) {
	if (args.size() < 1) {
		throw Errors::TooFewArgsException();
	}
	if (args[0].find(":") != std::string::npos) {
		if (!isNumber(args[0].substr(args[0].find(":") + 1)))
			throw std::invalid_argument("Port must be a number");
		_port = args[0].substr(args[0].find(":") + 1);
		_ip = args[0].substr(0, args[0].find(":"));
	}
	else {
		if (!isNumber(args[0]))
			throw std::invalid_argument("Port must be a number");
		_port = args[0];
		_ip = "localhost";
	}
}

Listen::~Listen() {}

const std::string&	Listen::getPort() const {
	return _port;
}

const std::string&	Listen::getIp() const {
	return _ip;
}