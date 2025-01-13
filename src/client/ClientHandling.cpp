#include "../../include/includes.hpp"
#include "../../include/includeClasses.hpp"
#include <cerrno>
#include <cstring>
#include <unistd.h>

int STATUS_CODE;

std::string	generateDirectoryListing(const std::string& urlPath, const std::string& root, Response* res) {
	std::string newRoot;
	std::string requestPath;

	if (root.find(urlPath) != std::string::npos) {
		newRoot = root.substr(0, root.find(urlPath));
		requestPath = getCurrentDir() + newRoot + urlPath;
	} else
		requestPath = getCurrentDir() + root + urlPath;

	DIR *dir = opendir(requestPath.c_str());
	if (!dir) {
		std::string response;
		STATUS_CODE = 403;
		getErrorPage(response, res, STATUS_CODE);
		return response;
	}

	std::ostringstream htmlResponse;
	htmlResponse << "<html><head><title>Index of " << urlPath << "</title></head><body>";
	htmlResponse << "<h1>Index of " << urlPath << "</h1><ul>";

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		std::string name = entry->d_name;

		if (name == "." || name == "..")
			continue;

		struct stat entryStat;
		std::string fullPath = root + urlPath + "/" + name;
		if (stat(fullPath.c_str(), &entryStat) == 0 && S_ISDIR(entryStat.st_mode))
			name += "/";

		htmlResponse << "<li><a href=\"" << name << "\">" << name << "</a></li>";
	}

	closedir(dir);

	htmlResponse << "</ul></body></html>";

	std::ostringstream response;
	response << "HTTP/1.1 200 OK\r\n";
	response << "Content-Type: text/html\r\n";
	response << "Content-Length: " << htmlResponse.str().size() << "\r\n\r\n";
	response << htmlResponse.str();

	return response.str();
}

void redirectingUrl(std::string index) {
	index += "";
	// index += "location: http://localhost:8080/redirect.html";
}

std::string    generateResponse(Request* req, Response* res) {
	std::string response, index, message, statusCode, contentType;
	std::ostringstream oss;

    contentType = "text/html";
	statusCode = int_to_string(STATUS_CODE);
    if (req->getUrlPath() == "/favicon.ico") {
        contentType = "image/x-icon";
    }
	if (req->getMethod() == "POST")
		contentType = "application/json";

	// TODO: Add status code for redirect
	switch (STATUS_CODE) {
		case 200:
			message = "OK";
			break;
		case 201:
			message = "Created";
			break;
		case 204:
			message = "No Content";
			break;
		case 400:
			message = "Bad Request";
			break;
		case 404:
			message = "Not Found";
			break;
		case 403:
			message = "Forbidden";
			break;
		case 405:
			message = "Method Not Allowed";
			break;
		case 500:
			message = "Internal Server Error";
			break;
		case 307:
			message = "Temporary redirect";
			break;
		default:
			break;
	}
	response = 
		"HTTP/1.1 " + statusCode + " " + message + "\r\n";

	if (STATUS_CODE == 307) {
		response += "Location: http://localhost:8080/saluto.html\r\n";
		redirectingUrl(index);
	}
	else if (STATUS_CODE == 200 && req->getMethod() != "POST")
		readHtml(index, req, res, STATUS_CODE);
	else if ((STATUS_CODE == 200 || STATUS_CODE == 201) && req->getMethod() == "POST")
		index = res->bodyToJson();
	else if (STATUS_CODE == 204)
		index = "";
	else
		getErrorPage(index, res, STATUS_CODE);

	response += "Content-Type: " + contentType + "\r\n"
		"Content-Length: ";
	oss << index.length();
	response += oss.str();
	response += "\r\n\r\n";
	response += index;

	return response;
}

void	lookForRequestType(Request* req, Http* http, Response* res, bool& locationExists) {
	std::string	serverName = http->getServerName(req->getHost());
	std::string	locationName = http->getLocationName(req->getUrlPath(), serverName);
	if (serverName.empty()) {
		STATUS_CODE = 404;
		res->setResponse(generateResponse(req, res));
		return;
	}
	if (locationName.empty())
		locationExists = false;

	setAllValues(res, http, serverName, locationName, locationExists);
}

void	clientHandler(int& clientSocket, Http* http) {
	// STATUS_CODE = 200;
	bool		locationExists = true;
	char		buffer[8192] = {0};
	Request		*request = new Request();
	Response	*res = new Response();
	std::string	response;

	// Receive data from the client
	int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (bytesReceived <= 0) {
		if (bytesReceived == 0)
			std::cout << "Client disconnected." << std::endl;
		else 
			std::cerr << "Error receiving data: " << strerror(errno) << std::endl;

		close(clientSocket); // Close the socket on error or disconnect
		clientSocket = -1;
		delete res;
		delete request;
		usleep(40000);
		return;
	}
	request->parseRequest(buffer);

	request->setClientId(clientSocket);
	lookForRequestType(request, http, res, locationExists);
	handleRequest(request, http, res, locationExists, STATUS_CODE);
	std::cout << "STATUS CODE AFTER HANDLE REQUEST: " << STATUS_CODE << std::endl;
	delete request;

	// Send the response to the client
	if (send(clientSocket, res->getResponse().c_str(), res->getResponse().size(), MSG_CONFIRM) <= 0) // needs a check with throw error
	{
		if (bytesReceived == 0)
			std::cout << "Error: " << strerror(errno) << std::endl;
		else
			std::cerr << "Error sending data: " << strerror(errno) << std::endl;
		close(clientSocket);
		clientSocket = -1;
	}
	delete res;
}