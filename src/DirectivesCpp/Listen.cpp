#include "../../include/includes.hpp"
#include "../../include/Directives/Listen.hpp"
#include "../../include/Errors.hpp"

Listen::Listen(const std::vector<std::string>& args) : AConfig(), _port("8080"), _ip("127.0.0.1") {
	if (args.size() < 1) {
		throw Errors::TooFewArgsException("Wrong number of args", __LINE__, __FILE__);
	}
	if (args[0].find(":") != std::string::npos) {
		std::string port = args[0].substr(args[0].find(":") + 1);
		std::string real_port = std::string(port.begin(), port.end() - 1);
		std::cout << "real_port: " << real_port << std::endl;
		if (!isNumber(real_port))
			throw Errors::InvalidArgumentException("Port must be a number", __LINE__, __FILE__);
		_port = real_port;
		_ip = args[0].substr(0, args[0].find(":"));
	}
	else {
		if (!isNumber(std::string(args[0].begin(), args[0].end() - 1)))
			throw Errors::InvalidArgumentException("Port must be a number", __LINE__, __FILE__);
		_port = args[0].substr(args[0][0], args[0].size() - 1);
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