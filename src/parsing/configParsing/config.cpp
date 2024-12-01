#include "../../../include/Errors.hpp"
#include "../../../include/Directives/config.hpp"
#include "../../../include/Directives/Http.hpp"
#include "../../../include/Directives/Listen.hpp"
#include "../../../include/Directives/Server.hpp"
#include "../../../include/Directives/ServerName.hpp"
#include "../../../include/Directives/Root.hpp"
#include "../../../include/Directives/Index.hpp"
#include "../../../include/Directives/ErrorPage.hpp"
#include "../../../include/Directives/ClientMaxBodySize.hpp"
#include "../../../include/Directives/Autoindex.hpp"
#include "../../../include/Directives/CgiPass.hpp"
#include "../../../include/Directives/Location.hpp"
#include "../../../include/Directives/Http.hpp"
#include "../../../include/Directives/Server.hpp"
#include "../../../include/includes.hpp"
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
			throw Errors::UnknownDirectiveException("Unknown directive", ConfigLine, __FILE__);
	}
	return NULL;
}

AConfig*	AConfig::createBlock(const std::string& directive, std::stringstream& file) {
	DirectiveType	type = getDirectiveType(directive);

	switch (type) {
		case SERVER:
			return new Server(file);
		case LOCATION:
			return new Location(file);
		default:
			throw Errors::UnknownDirectiveException("Unknown directive", ConfigLine, __FILE__);
	}
	return NULL;
}

void	AConfig::createDefaultDirectives(DirectiveType type) {
	switch (type) {
		case HTTP:
			if (this->_directives.find("client_max_body_size") == this->_directives.end())
				this->_directives["client_max_body_size"] = new ClientMaxBodySize();
			break;
		case SERVER:
			if (this->_directives.find("listen") == this->_directives.end())
				this->_directives["listen"] = new Listen();

			if (this->_directives.find("server_name") == this->_directives.end())
				this->_directives["server_name"] = new ServerName();

			if (this->_directives.find("error_page") == this->_directives.end()) {
				this->_directives["error_page" + to_string(400)] = new ErrorPage(400);
				this->_directives["error_page" + to_string(500)] = new ErrorPage(500);
			}
			break;
		case LOCATION:
			if (this->_directives.find("cgi_pass") == this->_directives.end())
				this->_directives["cgi_pass"] = new CgiPass();
			if (this->_directives.find("root") == this->_directives.end())
				this->_directives["root"] = new Root();

			if (this->_directives.find("index") == this->_directives.end())
				this->_directives["index"] = new Index();

			if (this->_directives.find("autoindex") == this->_directives.end())
				this->_directives["autoindex"] = new Autoindex();
			break;
		default:
			break;
	}
}

