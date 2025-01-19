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

	std::cout << "STATUS CODE in GENERATE RESPONSE: " << STATUS_CODE << std::endl;

    contentType = "text/html";
	statusCode = int_to_string(STATUS_CODE);
    if (req->getUrlPath() == "/favicon.ico") {
        contentType = "image/x-icon";
    }
		

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
		if (req->getUrlPath().substr(0, 9) != "/cgi-bin/")
		{
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

void parseMultiPartBody(std::vector<char> body, std::string header)
{
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

		int nameStart = part.find("filename=\"") + 9;
		int namSize = part.find("\"", nameStart - nameStart);
		int contentStart = part.find("Content-Type: ") + 14;
		int	contentSize = part.find("\r\n", contentStart - contentStart);
		fileName = part.substr(nameStart, namSize);
		fileType = part.substr(contentStart, contentSize);

		std::cout << "File name: " << fileName << std::endl;
		std::cout << "File type: " << fileType << std::endl;

		std::string toSub = part.substr(0, part.find("\r\n\r\n", 4) + 4);
		part = part.substr(toSub.length(), part.length());

        // std::ofstream fileUploaded(fileName.c_str(), std::ios::binary);
		// if (!fileUploaded.is_open())
		// 	std::cout << "Error: Failed to create file\n";
        // fileUploaded.write(part.c_str(), part.size() - 2); // Write the file content
		// if (fileUploaded.fail())
		// 	std::cout << "\n" << strerror(errno) << "\n";
		// fileUploaded.close();
		// break;

		std::ofstream *ofs;
		if (fileType.find("image"))
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
	// TODO: handle multiple content type with the cgi, not only the text/html
}

void	clientHandler(int& clientSocket, Http* http) {
	Request			*request = new Request();
	Response		*res = new Response();
	char			buffer[1];
	std::string		header;
	int				bytesReceived = 0;
	bool			locationExists = true;

	// Pick the header
	int totalBytes = 0;
	while (1) {
		bytesReceived = recv(clientSocket, buffer, 1, 0);
		if (bytesReceived <= 0)
		{
			if (bytesReceived == 0)
				std::cout << "Client disconnected." << std::endl;
			else
				std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
			close(clientSocket);
			clientSocket = -1;
			delete res;
			delete request;
			usleep(3);
			return;
		}
		totalBytes += bytesReceived; 
		header += buffer[0];
		if (header.find("\r\n\r\n") != std::string::npos)
			break;
	}
	request->parseRequest(header);
	// Pick the body if it exists
	if (std::string(header).find("Content-Type: ") != std::string::npos)
	{
		int totReceived = 0;
		int contentLength = atoi(request->getHeader("Content-Length").c_str());
		// if (contentLength > http->getDirective<Server>("server" + int_to_string(1)))
		std::vector<char> body(contentLength);
		while (totReceived < contentLength) {
        	int bytes_received = recv(clientSocket, body.data() + totReceived, contentLength - totReceived, 0);        	
			if (bytes_received <= 0) {
				if (bytes_received == 0)
					std::cout << "Client disconnected." << std::endl;
				close(clientSocket);
				clientSocket = -1;
				delete res;
				delete request;
				usleep(3);
				return;
        	}
        	totReceived += bytes_received;
    	}
		if (std::string(header).find("multipart/form-data;") != std::string::npos)
			parseMultiPartBody(body, header);
		else {
			std::string query = body.data();
			request->setQuery(query.substr(0, contentLength));
		}
	}

	request->setClientId(clientSocket);
	lookForRequestType(request, http, res, locationExists);
	handleRequest(request, http, res, locationExists, STATUS_CODE);
	// Send the response to the client
	if (send(clientSocket, res->getResponse().c_str(), res->getResponse().size(), MSG_CONFIRM) <= 0)
	{
		if (bytesReceived == 0)
			std::cout << "Error: client disconnected\n";
		close(clientSocket);
		clientSocket = -1;
		usleep(3);
	}
	delete request;
	delete res;
}