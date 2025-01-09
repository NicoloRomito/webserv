#include "../../include/includes.hpp"
#include "../../include/includeClasses.hpp"

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

std::string    generateResponse(Request* req, Response* res) {
	std::string response, index, message, statusCode, contentType;
	std::ostringstream oss;

    contentType = "text/html";
	statusCode = int_to_string(STATUS_CODE);
    if (req->getUrlPath() == "/favicon.ico") {
        contentType = "image/x-icon";
    }

	switch (STATUS_CODE) {
		case 200:
			message = "OK";
			break;
		case 204:
			message = "No Content";
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
		default:
			break;
	}
	response = 
		"HTTP/1.1 " + statusCode + " " + message + "\r\n"
		"Content-Type: " + contentType + "\r\n"
		"Content-Length: ";

	if (STATUS_CODE == 200)
		readHtml(index, req, res, STATUS_CODE);
	else if (STATUS_CODE == 204)
		index = "";
	else
		getErrorPage(index, res, STATUS_CODE);
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

void	clientHandler(int clientSocket, Http* http) {
	STATUS_CODE = 200;
	bool		locationExists = true;
	char		buffer[1024] = {0};
	Request		*request = new Request();
	Response	*res = new Response();
	std::string	response;

	// Receive data from the client
	int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (bytesReceived <= 0) {
		if (bytesReceived == 0)
			std::cout << "Client disconnected." << std::endl;
		else
			std::cerr << "Error receiving data." << std::endl;
		close(clientSocket); // Close the socket on error or disconnect
		return;
	}
	request->parseRequest(buffer);

	request->setClientId(clientSocket);
	lookForRequestType(request, http, res, locationExists);
	handleRequest(request, http, res, locationExists, STATUS_CODE);
	delete request;

	// Send the response to the client
	send(clientSocket, res->getResponse().c_str(), res->getResponse().size(), MSG_CONFIRM); // needs a check with throw error
	delete res;
}