#include "../../../include/Directives/Http.hpp"
#include "../../../include/Errors.hpp"
#include "../../../include/Directives/config.hpp"
#include "../../../include/includes.hpp"
#include <sstream>

std::vector<std::string>	returnLine(const std::string& line);

AConfig::AConfig() {}

AConfig::~AConfig() {}


AConfig*	AConfig::createDirective(const std::string& directive, std::vector<std::string> args) {
	DirectiveType	type = getDirectiveType(directive);

	switch (type) {
		case HTTP:
			return new Http();
		case LISTEN:
			return new Listen(args);
		case SERVER_NAME:
			return new ServerName(args);
		case ROOT:
			return new Root(args);
		case INDEX:
			return new Index(args);
		case ERROR_PAGE:
			return new ErrorPage(args);
		case CLIENT_MAX_BODY_SIZE:
			return new ClientMaxBodySize(args);
		case AUTOINDEX:
			return new Autoindex(args);
		case LOCATION:
			return new Location(args);
		case CGI_PASS:
			return new CgiPass(args);
		case SERVER:
			return new Server();
		default:
			error("Unknown directive");
	}
	return NULL;
}
