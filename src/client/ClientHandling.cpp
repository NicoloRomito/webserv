#include "../../include/includes.hpp"
#include "../../include/includeClasses.hpp"
#include <cerrno>
#include <cstddef>
#include <cstdlib>
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

	std::cout << "STATUS CODE in GENERATE RESPONSE: " << STATUS_CODE << std::endl;

    contentType = "text/html";
	statusCode = int_to_string(STATUS_CODE);
    if (req->getUrlPath() == "/favicon.ico") {
        contentType = "image/x-icon";
    }

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
		case 307:
			message = "Temporary redirect";
			break;
		case 400:
			message = "Bad Request";
			break;
		case 403:
			message = "Forbidden";
			break;
		case 404:
			message = "Not Found";
			break;
		case 405:
			message = "Method Not Allowed";
			break;
		case 413:
			message = "Content Too Large";
			break;
		case 422:
			message = "Unprocessable Content";
			break;
		case 500:
			message = "Internal Server Error";
			break;
		case 501:
			message = "Not Implemented";
			break;
		case 502:
			message = "Bad Gateway";
			break;
		default:
			break;
	}

	response = "HTTP/1.1 " + statusCode + " " + message + "\r\n";

	if (STATUS_CODE == 307) {
		response += "Location: http://localhost:8080/saluto.html\r\n";
		redirectingUrl(index);
	} else if (STATUS_CODE == 200 && req->getMethod() != "POST")
		readHtml(index, req, res, STATUS_CODE);
	else if (isValidPostReq(STATUS_CODE, req))
	{
		if (req->getUrlPath().substr(0, 9) != "/cgi-bin/") {
			contentType = "application/json";
			index = res->bodyToJson();
		}
		if (req->getUrlPath().substr(0, 9) == "/cgi-bin/" &&
		 	req->getUrlPath() != "/cgi-bin/")
			readHtml(index, req, res, STATUS_CODE);
	}
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

void parseMultiPartBody(std::vector<char> body, std::string header, Response* res)
{
	// take the boundary from the header dinamically
	int boundaryStart = header.find("boundary=") + 9;
	int boundarySize = header.find("boundary=") + 38 - header.find("boundary=");
	std::string boundary = "--" + header.substr(boundaryStart, boundarySize);
    // Now parse the body for parts
    std::string bodyStr(body.begin(), body.end());
	std::string fileName, filePath, fileType; 
    size_t pos = 0;
    
    // Keep looping to extract each part
    while ((pos = bodyStr.find(boundary, pos)) != std::string::npos) {
        size_t partStart = pos + boundary.length(); // Skip the boundary
        size_t partEnd = bodyStr.find(boundary, partStart);
        
        if (partEnd == std::string::npos) {
            break; // No more parts, we're at the end
        }
        // Extract the part (from partStart to partEnd)
        std::string part = bodyStr.substr(partStart, partEnd - partStart);

		fileName = parseFilename(part);
		fileType = parseFileType(part);

		std::cout << "File name: " << fileName << std::endl;
		std::cout << "File type: " << fileType << std::endl;

		std::string toSub = part.substr(0, part.find("\r\n\r\n", 4) + 4);
		part = part.substr(toSub.length(), part.length());

		fileName = res->getRoot() + "/uploads/" + fileName;

		std::ofstream *ofs;
		if (fileType.find("image") || fileType.find("pdf"))
        	ofs = new std::ofstream(fileName.c_str(), std::ios::binary);
        else 
			ofs = new std::ofstream(fileName.c_str());
		ofs->write(part.c_str(), part.size() - 2); // Write the file content
		if (ofs->fail())
			std::cout << "\n [UPLOAD] -> " << strerror(errno) << "\n";
		ofs->close();
		delete ofs;
        // Continue to the next part
        pos = partEnd;
    }
}

void	deleteAndSleep(Client *client, Request *request, Response *res) {
	client->closeSocket();
	delete client;
	delete request;
	delete res;
	usleep(3);
}

size_t	getCurrentMaxBodySize(Http* http, std::string currServer) {
	if (http->getDirective<Server>(currServer)->getDirective<ClientMaxBodySize>("client_max_body_size"))
		return http->getDirective<Server>(currServer)->getDirective<ClientMaxBodySize>("client_max_body_size")->getSize();
	else
		return http->getDirective<Http>("http")->getDirective<ClientMaxBodySize>("client_max_body_size")->getSize();
}

void	clientHandler(int& clientSocket, Http* http, std::string currServer) {
	Client			*client = new Client(clientSocket);
	Request			*request = new Request();
	Response		*res = new Response();
	bool			locationExists = true;
	size_t			MaxBodySize = getCurrentMaxBodySize(http, currServer);


	(void)currServer;
	// Pick the header
	if (client->readHeader() == -1) {
		deleteAndSleep(client, request, res);
		return;
	}
	request->parseRequest(client->getHeader());
	// TODO: check methods allowed

	if (client->getHeader().find("Content-Length: ") != std::string::npos)
		client->setContentLength(atoll(request->getHeader("Content-Length").c_str()));

	if (client->getHeader().find("Content-Type: ") != std::string::npos) {
		if (client->readBody() == -1) {
			deleteAndSleep(client, request, res);
			return;
		}
		if (client->getHeader().find("multipart/form-data;") != std::string::npos)
			parseMultiPartBody(client->getBody(), client->getHeader(), res);
		else {
			std::string query = client->getBody().data();
			request->setQuery(query.substr(0, client->getContentLength()));
		}
	}

	request->setClientId(client->getSocket());
	if (client->getContentLength() > MaxBodySize) {
		STATUS_CODE = 413;
		res->setResponse(generateResponse(request, res));
	} else {
		lookForRequestType(request, http, res, locationExists);
		handleRequest(request, http, res, locationExists, STATUS_CODE);
	}
	// Send the response to the client
	if (send(client->getSocket(), res->getResponse().c_str(), res->getResponse().size(), MSG_CONFIRM) <= 0)
	{
		std::cout << "Error: client disconnected\n";
		client->closeSocket();
		usleep(3);
	}
	delete client;
	delete request;
	delete res;
}