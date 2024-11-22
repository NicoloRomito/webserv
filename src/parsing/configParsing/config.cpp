#include "../../../include/Errors.hpp"
#include "../../../include/Directives/config.hpp"
#include "../../../include/includes.hpp"
#include "../../../include/Directives/Listen.hpp"
#include "../../../include/Directives/ServerName.hpp"
#include "../../../include/Directives/Root.hpp"
#include "../../../include/Directives/Index.hpp"
#include "../../../include/Directives/ErrorPage.hpp"
#include "../../../include/Directives/ClientMaxBodySize.hpp"
#include "../../../include/Directives/Autoindex.hpp"
#include "../../../include/Directives/CgiPass.hpp"
#include <sstream>

AConfig::AConfig() {}

AConfig::~AConfig() {}


AConfig*	AConfig::createDirective(const std::string& directive, std::vector<std::string> args) {
	DirectiveType	type = getDirectiveType(directive);

	switch (type) {
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
		case CGI_PASS:
			return new CgiPass(args);
		default:
			error("Unknown directive");
	}
	return NULL;
}
