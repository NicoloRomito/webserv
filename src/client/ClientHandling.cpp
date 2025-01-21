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
	std::cout << STATUS_CODE << std::endl;
	if (STATUS_CODE == 301) {
		std::string uri = req->getUri().substr(0, req->getUri().size() - req->getUrlPath().size());
		std::cout << "URI: " << uri << std::endl;
		response += "Location: " + uri + res->getRewriteReplacement() + "\r\n";
	}
	else if (STATUS_CODE == 307)
		response += "Location: " + res->getRewriteReplacement() + "\r\n";
	index = "";
}

const std::string setContentType(Request* req) {
	std::string contentType = "text/html";
	std::string extension = req->getUrlPath().substr(req->getUrlPath().find_last_of('.') + 1);
	std::cout << "Extension: " << extension << std::endl;
	if (extension == "html" || extension == "css")
		contentType = "text/" + extension;
	else if (extension == "js")
		contentType = "application/javascript";
	else if (extension == "jpg" || extension == "jpeg")
		contentType = "image/jpeg";
	else if (extension == "png")
		contentType = "image/png";
	else if (extension == "gif")
		contentType = "image/gif";
	else if (extension == "ico")
		contentType = "image/x-icon";
	else if (extension == "json")
		contentType = "application/json";
	else if (extension == "xml")
		contentType = "application/xml";
	else if (extension == "pdf")
		contentType = "application/pdf";
	else if (extension == "zip")
		contentType = "application/zip";
	else if (extension == "tar")
		contentType = "application/x-tar";
	else if (extension == "gz")
		contentType = "application/x-gzip";
	else if (extension == "mp3")
		contentType = "audio/mpeg";
	else if (extension == "mp4")
		contentType = "video/mp4";
	else if (extension == "mpeg")
		contentType = "video/mpeg";
	else if (extension == "webm")
		contentType = "video/webm";
	else if (extension == "ogg")
		contentType = "video/ogg";
	else if (extension == "wav")
		contentType = "audio/wav";
	else if (extension == "avi")
		contentType = "video/x-msvideo";
	else if (extension == "txt")
		contentType = "text/plain";
	return contentType;
}

std::string    generateResponse(Request* req, Response* res) {
	std::string response, index, message, statusCode, contentType;
	std::ostringstream oss;

	bool isRedirect = false;
	contentType = setContentType(req);
	statusCode = int_to_string(STATUS_CODE);

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
		case 301:
			message = "Moved Permanently";
			isRedirect = true;
			break;
		case 307:
			message = "Temporary redirect";
			isRedirect = true;
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
	// TODO: update error.html with nicer code and refactor this function

	response = "HTTP/1.1 " + statusCode + " " + message + "\r\n";

	std::cout << "Content type: " << contentType << std::endl;
	std::cout << "URI in generateResponse: " << req->getUri() << std::endl;
	if (isRedirect) {
		std::cout << "REDIRECTING URL" << std::endl;
		redirectingUrl(index, req, res, response);
		std::cout << "INDEX: " << index << std::endl;
	} else if (STATUS_CODE == 200 && req->getMethod() != "POST" && contentType == "text/html")
		readHtml(index, req, res, STATUS_CODE);
	else if (STATUS_CODE == 200 && contentType != "text/html")
		readFile(index, res, STATUS_CODE);
	else if (isValidPostReq(STATUS_CODE, req))
	{
		if (req->getUrlPath().substr(0, 9) != "/cgi-bin/") {
			contentType = "application/json";
			index = res->bodyToJson();
		}
		if (req->getUrlPath().substr(0, 9) == "/cgi-bin/" &&
		 	req->getUrlPath() != "/cgi-bin/")
			readHtml(index, req, res, STATUS_CODE);
	} else if (STATUS_CODE == 204)
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

		std::cout << "File name: " << fileName << std::endl;
		std::cout << "File type: " << fileType << std::endl;

		std::string toSub = part.substr(0, part.find("\r\n\r\n", 4) + 4);
		part = part.substr(toSub.length(), part.length());

		filePath = "./src/www/static/uploads/" + fileName;

		std::ofstream *ofs;
		ofs = new std::ofstream(filePath.c_str(), std::ios::binary);
		ofs->write(part.c_str(), part.size() - 2); // Write the file content
		if (ofs->fail())
		{
			std::cout << "\n [UPLOAD] -> " << strerror(errno) << "\n";
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
	client->closeSocket();
	delete client;
	delete request;
	delete res;
	delete up;
	usleep(3);
}

size_t	getCurrentMaxBodySize(Http* http, Request* req, std::string currServer) {
	std::string	locationName = http->getLocationName(req->getUrlPath(), currServer);
	if (!locationName.empty() && 
		http->getDirective<Server>(currServer)->getDirective<Location>(locationName)->getDirective<ClientMaxBodySize>("client_max_body_size"))
		return http->getDirective<Server>(currServer)->getDirective<Location>(locationName)->getDirective<ClientMaxBodySize>("client_max_body_size")->getSize();
	else if (http->getDirective<Server>(currServer)->getDirective<ClientMaxBodySize>("client_max_body_size"))
		return http->getDirective<Server>(currServer)->getDirective<ClientMaxBodySize>("client_max_body_size")->getSize();
	else 
		return http->getDirective<Http>("http")->getDirective<ClientMaxBodySize>("client_max_body_size")->getSize();
}

void	clientHandler(int& clientSocket, Http* http, std::string currServer) {
	Client			*client = new Client(clientSocket);
	Upload			*uploadRes = new Upload();
	Request			*request = new Request();
	Response		*res = new Response();
	bool			locationExists = true;


	// Pick the header
	if (client->readHeader() == -1) {
		deleteAndSleep(client, request, res, uploadRes);
		return;
	}
	request->parseRequest(client->getHeader());

	size_t	MaxBodySize = getCurrentMaxBodySize(http, request, currServer);
	std::cout << "Max body size: " << MaxBodySize << std::endl;
	if (client->getHeader().find("Content-Length: ") != std::string::npos)
		client->setContentLength(atoll(request->getHeader("Content-Length").c_str()));

	if (client->getHeader().find("Content-Type: ") != std::string::npos && client->getContentLength() <= MaxBodySize) {
		bool isMultipart = client->getHeader().find("multipart/form-data;") != std::string::npos;
		if (client->readBody(isMultipart) == -1) {
			deleteAndSleep(client, request, res, uploadRes);
			return;
		}
		if (isMultipart)
		{
			parseMultiPartBody(client->getBody(), client->getHeader(), uploadRes);
		}
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
		handleRequest(request, http, res, locationExists, STATUS_CODE, uploadRes);
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
	delete uploadRes;
}