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
#include <exception>
#include <iostream>
#include <sstream>

AConfig::AConfig() {}

AConfig::~AConfig() {}


AConfig*	AConfig::createDirective(const std::string& directive, std::vector<std::string> args) {
	DirectiveType	type = getDirectiveType(directive);

	switch (type) {
		case LISTEN: {
			Listen *listen = new Listen();
			listen->parseDirective(args);
			return listen;
		} case SERVER_NAME: {
			ServerName *serverName = new ServerName();
			serverName->parseDirective(args);
			return serverName;
		} case ROOT: {
			Root *root = new Root();
			root->parseDirective(args);
			return root;
		} case INDEX: {
			Index *index = new Index();
			index->parseDirective(args);
			return index;
		} case ERROR_PAGE: {
			ErrorPage *errorPage = new ErrorPage();
			errorPage->parseDirective(args);
			return errorPage;
		} case CLIENT_MAX_BODY_SIZE: {
			ClientMaxBodySize *clientMaxBodySize = new ClientMaxBodySize();
			clientMaxBodySize->parseDirective(args);
			return clientMaxBodySize;
		} case AUTOINDEX: {
			Autoindex *autoindex = new Autoindex();
			autoindex->parseDirective(args);
			return autoindex;
		} case CGI_PASS: {
			CgiPass *cgiPass = new CgiPass();
			cgiPass->parseDirective(args);
			return cgiPass;
		} default:
			throw Errors::UnknownDirectiveException("Unknown directive", ConfigLine, __FILE__);
	}
	return NULL;
}

AConfig*	AConfig::createBlock(const std::string& directive, std::stringstream& file, const std::string& locationPath) {
	DirectiveType	type = getDirectiveType(directive);

	Server *server = NULL;
	Location *location = NULL;

	try {	
		switch (type) {
			case SERVER: {
				server = new Server();
				server->parse(file);
				return server;
			} case LOCATION: {
				location = new Location(locationPath);
				location->parse(file);
				return location;
			} default:
				throw Errors::UnknownDirectiveException("Unknown directive", ConfigLine, __FILE__);
		}
	} catch (std::exception& e) {
		delete server;
		delete location;

		throw;
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

			// if (this->_directives.find("server_name") == this->_directives.end())
			// 	this->_directives["server_name"] = new ServerName();

			if (this->_directives.find(this->_dName + "error_page4xx") == this->_directives.end())
				this->_directives[this->_dName + "error_page4xx"] = new ErrorPage(400);
			if (this->_directives.find(this->_dName + "error_page5xx") == this->_directives.end())
				this->_directives[this->_dName + "error_page5xx"] = new ErrorPage(500);
			break;
		case LOCATION:
			if (this->_directives.find("cgi_pass") == this->_directives.end()) {
				CgiPass *cgiPass = new CgiPass();
				cgiPass->setDefaultCgiPaths();
				this->_directives["cgi_pass"] = cgiPass;
			}
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

void	AConfig::cleanDirectives() {
	std::map<std::string, AConfig*>::iterator it = this->_directives.begin();
	for (; it != this->_directives.end(); it++) {
		// std::cout << "Deleting directive: " << it->first << std::endl;
		if (it->second)
			delete it->second;
	}
	this->_directives.clear();
}


bool	AConfig::alreadyExists(const std::string& directiveName) const {
	std::map<std::string, AConfig *>::const_iterator it = _directives.find(directiveName);

	if (it != _directives.end())
		return true;
	return false;
}