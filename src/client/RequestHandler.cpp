#include "../../include/includes.hpp"
#include "../../include/includeClasses.hpp"
#include <unistd.h>

bool	isAllowedMethod(Request* req, Response* res) {
	std::set<std::string> allowedMethods = res->getAllowedMethods();
	for (std::set<std::string>::iterator it = allowedMethods.begin(); it != allowedMethods.end(); it++) {
		if (*it == req->getMethod())
			return true;
	}
	return false;
}

void	handleRequest(Request* request, Response* res, bool locationExists, int& statusCode, Upload *upload) {
	if (request->getMethod() == "GET" && isAllowedMethod(request, res)) {
		handleGet(request, res, locationExists, statusCode);
	} else if (request->getMethod() == "POST" && isAllowedMethod(request, res)) {
		handlePost(request, res, statusCode, upload);
	} else if (request->getMethod() == "DELETE" && isAllowedMethod(request, res)) {
		handleDelete(request, res, statusCode);
	} else {
		statusCode = 501;
		res->setResponse(generateResponse(request, res));
	}
}

bool	indexExists(const std::string& cwd, const std::string& root, const std::string& index) {
	if (index.empty())
		return false;
	std::string path = cwd + root + "/" + index;
	if (isADirectory(index, root))
	{
		return false;
	}
	if (access(path.c_str(), F_OK) == 0)
		return true;
	return false;
}

void	handleGet(Request* req, Response* res, bool locationExists, int& statusCode) {
	statusCode = 200;

	std::string	cwd = getCurrentDir();
	if (cwd.empty()) {
		statusCode = 500;
		res->setResponse(generateResponse(req, res));
		return;
	}

	if (req->getUrlPath().find("/favicon.ico") != std::string::npos) {
		std::string path;
		if (req->getUrlPath() == "/favicon.ico")
			path = cwd + res->getRoot() + "/icons" + req->getUrlPath();
		else
			path = cwd + res->getRoot() + req->getUrlPath();
		res->setPathForHtml(path);
		res->setResponse(generateResponse(req, res));
		return;
	}

	// REDIRECTION //
	if (req->getUrlPath() == res->getRewriteToReplace()) {
		if (res->getRewriteFlag() == "redirect")
			statusCode = 307;
		else if (res->getRewriteFlag() == "permanent")
			statusCode = 301;
		res->setResponse(generateResponse(req, res));
		return;
	}

	if (isADirectory(req->getUrlPath(), res->getRoot())) {
		// handle directory if location and index exist,
		if (locationExists && locationMatches(req->getUrlPath(), res->getLocationPath())
			&& indexExists(cwd, res->getRoot(), res->getIndex())) {
			res->setPathForHtml(cwd + res->getRoot() + "/" + res->getIndex());
		} // handle directory if location and autoindex exist
		else if (locationExists && locationMatches(req->getUrlPath(), res->getLocationPath()) && res->getAutoindex() == true) {
			res->setResponse(generateDirectoryListing(req->getUrlPath(), res->getRoot(), res));
			return;
		} else if (locationExists && locationMatches(req->getUrlPath(), res->getLocationPath()) && !res->getAutoindex()) {
			statusCode = 403;
			res->setResponse(generateResponse(req, res));
			return;
		} else {
			if (indexExists(cwd, res->getRoot(), res->getIndex())) {
				res->setPathForHtml(cwd + res->getRoot() + "/" + res->getIndex());
			} else if (res->getAutoindex() == true) {
				res->setResponse(generateDirectoryListing(req->getUrlPath(), res->getRoot(), res));
				return;
			} else {
				statusCode = 403;
				res->setResponse(generateResponse(req, res));
				return;
			}
		}
	}
	else { // handle file
		res->setPathForHtml(std::string(cwd) + res->getRoot() + req->getUrlPath());
	}

	if (access(res->getPathForHtml().c_str(), F_OK) == -1 &&
		res->getPathForHtml().find("/cgi-bin/") == std::string::npos)
	{
		statusCode = 404;
		res->setResponse(generateResponse(req, res));
		return;
	}
	res->setResponse(generateResponse(req, res));
}

void	handleDelete(Request* req, Response* res, int& statusCode) {
	std::string cwd = getCurrentDir();
	if (cwd.empty()) {
		statusCode = 500;
		res->setResponse(generateResponse(req, res));
		return;
	}
	std::string path = cwd + res->getRoot() + req->getUrlPath();

	if (isADirectory(req->getUrlPath(), res->getRoot())) {
		statusCode = 403;
		res->setResponse(generateResponse(req, res));
		return;
	}
	if (remove(path.c_str()) != 0) {
		statusCode = 404;
		res->setResponse(generateResponse(req, res));
	} else {
		statusCode = 204;
		res->setResponse(generateResponse(req, res));
	}
}
