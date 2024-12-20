#include "../headers/Response.hpp"

Response::Response() : _statusCode(0), _response(""), _root(""), _autoindex(""), _index(""), _errorPage4xx(""), _errorPage5xx("") {}

Response::~Response() {}

	// --------------------- //
	// -------SETTERS------- //
	// --------------------- //

void Response::setStatusCode(int statusCode) {
	_statusCode = statusCode;
}

void Response::setResponse(const std::string& response) {
	_response = response;
}

void Response::setRoot(const std::string& root) {
	_root = root;
}

void Response::setAutoindex(const std::string& autoindex) {
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

int Response::getStatusCode() const {
	return _statusCode;
}

const std::string& Response::getResponse() const {
	return _response;
}

const std::string& Response::getRoot() const {
	return _root;
}

const std::string& Response::getAutoindex() const {
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

