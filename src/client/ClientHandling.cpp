#include "../../include/includes.hpp"
#include "../../include/includeClasses.hpp" // IWYU pragma: keep
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

void redirectingUrl(std::string& index, Request* req, Response* res, std::string& response) {
	if (STATUS_CODE == 301) {
		std::string uri = req->getUri().substr(0, req->getUri().size() - req->getUrlPath().size());
		response += "Location: " + uri + res->getRewriteReplacement() + "\r\n";
	}
	else if (STATUS_CODE == 307)
		response += "Location: " + res->getRewriteReplacement() + "\r\n";
	index = "";
}

std::string    generateResponse(Request* req, Response* res) {
	std::string response, index, message, statusCode, contentType;
	std::ostringstream oss;

	bool isRedirect = false;
	contentType = setContentType(req);
	statusCode = int_to_string(STATUS_CODE);
	setMessageandRedirect(message, STATUS_CODE, isRedirect);

	response = "HTTP/1.1 " + statusCode + " " + message + "\r\n";

	if (isRedirect)
	{
		redirectingUrl(index, req, res, response);
	}
	else if (STATUS_CODE == 200 && req->getMethod() != "POST" && contentType == "text/html")
	{
		readHtml(index, req, res, STATUS_CODE);
	}
	else if (STATUS_CODE == 200 && contentType != "text/html")
	{
		readFile(index, res, STATUS_CODE);
	}
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

void parseMultiPartBody(std::vector<char> body, std::string header, Upload* uploadRes)
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

		std::string toSub = part.substr(0, part.find("\r\n\r\n", 4) + 4);
		part = part.substr(toSub.length(), part.length());

		filePath = "./src/www/static/uploads/" + fileName;

		std::ofstream *ofs;
		ofs = new std::ofstream(filePath.c_str(), std::ios::binary);
		ofs->write(part.c_str(), part.size() - 2); // Write the file content
		if (ofs->fail())
		{
			printError("Upload failed.");
			ofs->close();
			delete ofs;
			uploadRes->setFailure();
			return ;
		}
		ofs->close();
		delete ofs;
        // Continue to the next part
        pos = partEnd;
    }
	uploadRes->setSuccess(fileName, fileType);
}

void	deleteAndSleep(Client *client, Request *request, Response *res, Upload* up) {
	delete client;
	delete request;
	delete res;
	delete up;
}

size_t	getCurrentMaxBodySize(Http* http, Request* req, std::string currServer) {
	std::string	locationName = http->getLocationName(req->getUrlPath(), currServer);
	if (!locationName.empty() && 
		http->getDirective<Server>(currServer)->getDirective<Location>(locationName)->getDirective<ClientMaxBodySize>("client_max_body_size"))
		return http->getDirective<Server>(currServer)->getDirective<Location>(locationName)->getDirective<ClientMaxBodySize>("client_max_body_size")->getSize();
	else if (http->getDirective<Server>(currServer)->getDirective<ClientMaxBodySize>("client_max_body_size"))
		return http->getDirective<Server>(currServer)->getDirective<ClientMaxBodySize>("client_max_body_size")->getSize();
	else 
		return http->getDirective<ClientMaxBodySize>("client_max_body_size")->getSize();
}

void	clientHandler(int& clientSocket, Http* http, std::string currServer) {
	Client			*client = new Client();
	Upload			*uploadRes = new Upload();
	Request			*request = new Request();
	Response		*res = new Response();
	bool			locationExists = true;

	// Pick the header
	if (client->readHeader(clientSocket) == -1) {
		deleteAndSleep(client, request, res, uploadRes);
		return;
	}
	request->parseRequest(client->getHeader());

	const size_t	MaxBodySize = getCurrentMaxBodySize(http, request, currServer);
	if (client->getHeader().find("Content-Length: ") != std::string::npos)
		client->setContentLength(atoll(request->getHeader("Content-Length").c_str()));

	if (client->getHeader().find("Content-Type: ") != std::string::npos && client->getContentLength() <= MaxBodySize) {
		bool isMultipart = client->getHeader().find("multipart/form-data;") != std::string::npos;
		if (client->readBody(isMultipart, clientSocket) == -1) {
			deleteAndSleep(client, request, res, uploadRes);
			return;
		}
		if (isMultipart)
		{
			parseMultiPartBody(client->getBody(), client->getHeader(), uploadRes);
		}
		else
		{
			const char* tmp = client->getBody().data();
			std::string query(tmp, client->getBody().size());
			request->setQuery(query.substr(0, client->getContentLength()));
		}
	}

	request->setClientId(clientSocket);
	
	lookForRequestType(request, http, res, locationExists);
	if (client->getContentLength() > MaxBodySize) {
		STATUS_CODE = 413;
		res->setResponse(generateResponse(request, res));
	} else {
		handleRequest(request, res, locationExists, STATUS_CODE, uploadRes);
	}
	// Send the response to the client
	if (send(clientSocket, res->getResponse().c_str(), res->getResponse().size(), MSG_CONFIRM) <= 0)
	{
		std::cout << "Error: client disconnected\n";
		close(clientSocket);
		clientSocket = -1;
		usleep(3);
	}
	delete client;
	delete request;
	delete res;
	delete uploadRes;
}