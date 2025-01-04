#include "../headers/Response.hpp"
#include <vector>

Response::Response() : _response(""), _root(""), _locationPath(""), _autoindex(), _index(""), _errorPage4xx(""), _errorPage5xx(""), _pathForHtml("") {}

Response::~Response() {}

bool Response::isAvailableErrorCode(int code) const {
	for (std::vector<int>::const_iterator it = _statusCodes.begin(); it != _statusCodes.end(); ++it) {
		if (*it == code)
			return true;
	}
	return false;
}

	// --------------------- //
	// -------SETTERS------- //
	// --------------------- //

void	Response::setAvailableErrorCodes(const std::vector<int>& clientCodes, const std::vector<int>& serverCodes) {
	_statusCodes.insert(_statusCodes.end(), clientCodes.begin(), clientCodes.end());
	_statusCodes.insert(_statusCodes.end(), serverCodes.begin(), serverCodes.end());
}

void	Response::setCgiPass(const std::vector<std::string>& cgiPass) {
	_cgiPass = cgiPass;
}

void	Response::setServerNames(const std::vector<std::string>& serverNames) {
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

void Response::setResponse(const std::string& response) {
	_response = response;
}

void Response::setRoot(const std::string& root) {
	_root = root;
}

void Response::setAutoindex(bool autoindex) {
	_autoindex = autoindex;
}

void Response::setIndex(const std::string& index) {
	_index = index;
}

void Response::setErrorPage4xx(const std::string& error_page4xx) {
	_errorPage4xx = error_page4xx;
}

void Response::setErrorPage5xx(const std::string& error_page5xx) {
	_errorPage5xx = error_page5xx;
}

	// --------------------- //
	// -------GETTERS------- //
	// --------------------- //

const std::vector<int> Response::getAvailableErrorCodes() const {
	return _statusCodes;
}

const std::string& Response::getLocationPath() const {
	return _locationPath;
}

const std::string& Response::getPathForHtml() const {
	return _pathForHtml;
}

const std::string& Response::getPathForCgiScript() const {
	return _pathForCgiScript;
}

const std::string& Response::getResponse() const {
	return _response;
}

const std::string& Response::getRoot() const {
	return _root;
}

bool Response::getAutoindex() const {
	return _autoindex;
}

const std::string& Response::getIndex() const {
	return _index;
}

const std::string& Response::getErrorPage4xx() const {
	return _errorPage4xx;
}

const std::string& Response::getErrorPage5xx() const {
	return _errorPage5xx;
}

const std::vector<std::string> Response::getCgiPass() const {
	return _cgiPass;
}

const std::vector<std::string> Response::getServerNames() const {
	return _serverNames;
}