#include "../../include/includes.hpp"
#include "../../include/includeClasses.hpp"
#include <unistd.h>

bool	isAllowedMethod(Request* req, Response* res) {
	if (res->getAllowedMethods().find(req->getMethod()) == res->getAllowedMethods().end())
		return false;
	return true;
}

void	handleRequest(Request* request, Http* http, Response* res, bool locationExists, int& statusCode, Upload *upload) {
	(void)http;
	if (request->getMethod() == "GET" && isAllowedMethod(request, res)) {
		handleGet(request, res, locationExists, statusCode);
	} else if (request->getMethod() == "POST" && isAllowedMethod(request, res)) {
		handlePost(request, res, statusCode, upload);
	} else if (request->getMethod() == "DELETE") {
		handleDelete(request, res, statusCode);
	} else {
		statusCode = 405;
		res->setResponse(generateResponse(request, res));
	}
}

void	handleGet(Request* req, Response* res, bool locationExists, int& statusCode) {
	std::string cwd = getCurrentDir();
	if (cwd.empty()) {
		statusCode = 500;
		res->setResponse(generateResponse(req, res));
		return;
	}

	statusCode = 200;

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
		if (locationExists && locationMatches(req->getUrlPath(), res->getLocationPath()) && !res->getIndex().empty()) {
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
			if (!res->getIndex().empty()) {
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
