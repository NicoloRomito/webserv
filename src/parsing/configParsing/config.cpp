#include "../../../include/includeClasses.hpp" // IWYU pragma: keep
#include "../../../include/includes.hpp"
#include <cstddef>
#include <exception>

AConfig::AConfig() {}

AConfig::~AConfig() {}


AConfig*	AConfig::createDirective(const std::string& directive, std::vector<std::string> args) {
	DirectiveType	type = getDirectiveType(directive);
	Listen *listen = NULL;
	ServerName *serverName = NULL;
	Root *root = NULL;
	Index *index = NULL;
	ErrorPage *errorPage = NULL;
	ClientMaxBodySize *clientMaxBodySize = NULL;
	Autoindex *autoindex = NULL;
	CgiPass *cgiPass = NULL;
	AllowMethods *allowMethods = NULL;
	Rewrite *rewrite = NULL;

	try {
		switch (type) {
			case LISTEN: {
				listen = new Listen();
				listen->parseDirective(args);
				return listen;
			} case SERVER_NAME: {
				serverName = new ServerName();
				serverName->parseDirective(args);
				return serverName;
			} case ROOT: {
				root = new Root();
				root->parseDirective(args);
				return root;
			} case INDEX: {
				index = new Index();
				index->parseDirective(args);
				return index;
			} case ERROR_PAGE: {
				errorPage = new ErrorPage();
				errorPage->parseDirective(args);
				return errorPage;
			} case CLIENT_MAX_BODY_SIZE: {
				clientMaxBodySize = new ClientMaxBodySize();
				clientMaxBodySize->parseDirective(args);
				return clientMaxBodySize;
			} case AUTOINDEX: {
				autoindex = new Autoindex();
				autoindex->parseDirective(args);
				return autoindex;
			} case CGI_PASS: {
				cgiPass = new CgiPass();
				cgiPass->parseDirective(args);
				return cgiPass;
			} case ALLOW_METHODS: {
				allowMethods = new AllowMethods();
				allowMethods->setMethods(args);
				return allowMethods;
			} case REWRITE: {
				rewrite = new Rewrite();
				rewrite->parseDirective(args);
				return rewrite;
			} default:
				throw Errors::UnknownDirectiveException("Unknown directive", ConfigLine, __FILE__);
		}
	} catch (std::exception& e) {
		delete listen;
		delete serverName;
		delete root;
		delete index;
		delete errorPage;
		delete clientMaxBodySize;
		delete autoindex;
		delete cgiPass;
		delete allowMethods;
		delete rewrite;

		throw;
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
			if (this->_directives.find("allow_methods") == this->_directives.end())
				this->_directives["allow_methods"] = new AllowMethods("GET", "POST", "DELETE");
			break;
		case SERVER:
			if (this->_directives.find("listen") == this->_directives.end())
				this->_directives["listen"] = new Listen();
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