#include "../headers/Response.hpp"
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

Response::Response() : _clientMaxBodySize(0), _response(""), _root(""), _locationPath(""), _autoindex(), _index(""), _errorPage4xx(""), _errorPage5xx(""), _pathForHtml("") {}

Response::~Response() {
	_serverNames.clear();
	_statusCodes.clear();
	_cgiPass.clear();
	_body.clear();
}

bool Response::isAvailableErrorCode(int code) const {
	for (std::set<int>::const_iterator it = _statusCodes.begin(); it != _statusCodes.end(); ++it) {
		if (*it == code)
			return true;
	}
	return false;
}

typedef std::map<std::string, std::string>::iterator mapIt;

std::string Response::bodyToJson() {
	std::map<std::string, std::string> body = this->_body;
	std::string json = 	"{\r\n";

	for (mapIt it = body.begin(); it != body.end(); ++it)
		json += "  \"" + it->first + "\": " + it->second+ ",\r\n";
	json += "}\r\n";

	return json;
}

	// --------------------- //
	// -------SETTERS------- //
	// --------------------- //

void	Response::setAvailableErrorCodes(const std::set<int>& clientCodes, const std::set<int>& serverCodes) {
	for (std::set<int>::const_iterator it = clientCodes.begin(); it != clientCodes.end(); ++it) {
		_statusCodes.insert(*it);
	}
	for (std::set<int>::const_iterator it = serverCodes.begin(); it != serverCodes.end(); ++it) {
		_statusCodes.insert(*it);
	}
}

void	Response::setClientMaxBodySize(size_t clientMaxBodySize) {
	_clientMaxBodySize = clientMaxBodySize;
}

void	Response::setAllowedMethods(const std::set<std::string>& allowedMethods) {
	_allowedMethods = allowedMethods;
}

void	Response::setCgiPass(const std::vector<std::string>& cgiPass) {
	_cgiPass = cgiPass;
}

void	Response::setServerNames(const std::set<std::string>& serverNames) {
	_serverNames = serverNames;
}

void	Response::setLocationPath(const std::string& locationPath) {
	_locationPath = locationPath;
}

void	Response::setPathForHtml(const std::string& pathForHtml) {
	_pathForHtml = pathForHtml;
}

void	Response::setPathForCgiScript(const std::string& pathForCgiScript) {
	_pathForCgiScript = pathForCgiScript;
}

void	Response::setResponse(const std::string& response) {
	_response = response;
}

void	Response::setRoot(const std::string& root) {
	_root = root;
}

void	Response::setAutoindex(bool autoindex) {
	_autoindex = autoindex;
}

void	Response::setIndex(const std::string& index) {
	_index = index;
}

void	Response::setErrorPage4xx(const std::string& error_page4xx) {
	_errorPage4xx = error_page4xx;
}

void 	Response::setBody(const std::map<std::string, std::string> body) {
	this->_body = body;
}

void	Response::setErrorPage5xx(const std::string& error_page5xx) {
	_errorPage5xx = error_page5xx;
}

void	Response::setRewriteToReplace(const std::string& toReplace) {
	_rewriteToReplace = toReplace;
}

void	Response::setRewriteReplacement(const std::string& replacement) {
	_rewriteReplacement = replacement;
}

void	Response::setRewriteFlag(const std::string& flag) {
	_rewriteFlag = flag;
}

	// --------------------- //
	// -------GETTERS------- //
	// --------------------- //

const	std::set<int>	Response::getAvailableErrorCodes() const {
	return _statusCodes;
}

const	std::set<std::string>	Response::getAllowedMethods() const {
	return _allowedMethods;
}

size_t	Response::getClientMaxBodySize() const {
	return _clientMaxBodySize;
}

const	std::string& Response::getLocationPath() const {
	return _locationPath;
}

const	std::string& Response::getPathForHtml() const {
	return _pathForHtml;
}

const	std::string& Response::getPathForCgiScript() const {
	return _pathForCgiScript;
}

const	std::string& Response::getResponse() const {
	return _response;
}

const	std::string& Response::getRoot() const {
	return _root;
}

bool	Response::getAutoindex() const {
	return _autoindex;
}

const	std::string& Response::getIndex() const {
	return _index;
}

const	std::string& Response::getErrorPage4xx() const {
	return _errorPage4xx;
}

const	std::string& Response::getErrorPage5xx() const {
	return _errorPage5xx;
}

const	std::vector<std::string> Response::getCgiPass() const {
	return _cgiPass;
}

const	std::set<std::string> Response::getServerNames() const {
	return _serverNames;
}

const std::map<std::string, std::string> Response::getBody() const {
	return this->_body;
}

const std::string& Response::getRewriteToReplace() const {
	return _rewriteToReplace;
}

const std::string& Response::getRewriteReplacement() const {
	return _rewriteReplacement;
}

const std::string& Response::getRewriteFlag() const {
	return _rewriteFlag;
}