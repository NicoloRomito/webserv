#include "../../../include/Errors.hpp"
#include "../../../include/Directives/config.hpp"
#include "../../../include/includes.hpp"
#include "../../../include/Directives/Listen.hpp"
#include "../../../include/Directives/Server.hpp"
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
			throw Errors::UnknownDirectiveException("Unknown directive", __LINE__, __FILE__);
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
			throw Errors::UnknownDirectiveException("Unknown directive", __LINE__, __FILE__);
	}
	return NULL;
}

void	AConfig::createDefaultDirectives(DirectiveType type) {
	switch (type) {
		case HTTP:
			if (this->_directives.find("client_max_body_size") == this->_directives.end()) {
				std::vector<std::string> args;
				args.push_back("1m");
				this->_directives["client_max_body_size"] = new ClientMaxBodySize(args);
			}
			break;
		case SERVER:
			if (this->_directives.find("listen") == this->_directives.end()) {
				std::vector<std::string> args;
				args.push_back("80");
				this->_directives["listen"] = new Listen(args);
			}
			if (this->_directives.find("server_name") == this->_directives.end()) {
				std::vector<std::string> args;
				args.push_back("localhost");
				this->_directives["server_name"] = new ServerName(args);
			}
			if (this->_directives.find("error_page") == this->_directives.end()) {
				std::vector<std::string> args;
				args.push_back("404");
				args.push_back("/404.html");
				this->_directives["error_page"] = new ErrorPage(args);
			}
			break;
		case LOCATION:
			if (this->_directives.find("root") == this->_directives.end()) {
				std::vector<std::string> args;
				args.push_back("./");
				this->_directives["root"] = new Root(args);
			}
			if (this->_directives.find("index") == this->_directives.end()) {
				std::vector<std::string> args;
				args.push_back("index.html");
				this->_directives["index"] = new Index(args);
			}
			if (this->_directives.find("autoindex") == this->_directives.end()) {
				std::vector<std::string> args;
				args.push_back("off");
				this->_directives["autoindex"] = new Autoindex(args);
			}
			break;
		default:
			break;
	}
}
