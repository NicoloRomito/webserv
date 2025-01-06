#include "include/includes.hpp"
#include "include/includeClasses.hpp"
#include "src/classes/headers/Request.hpp"
#include "src/classes/headers/Response.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <dirent.h>

// * NOTE: This is a list of the directives that need to be called with numbers
// * at the end of the directive name. Here is the list:
// * Server -> server1, server2, server3, ...
// * If you have to call a directive inside server block, you have to call "server1root", "server1error_page4xx", "server1error_page5xx", ...
// * Location -> location1, location2, location3, ...
// * If you have to call a directive inside location block, you have to call "location1root", "location1error_page4xx", "location1error_page5xx", ...
// * Listen -> listen1, listen2, listen3, ...
// * ErrorPage -> server1error_page4xx, server1error_page5xx, ...

// ! NOTE: DEFAULT DIRECTIVES ARE:
// * Http: client_max_body_size;
// * Server: listen, server_name, error_page4xx, error_page5xx;
// * Location: cgi_pass, root, index, autoindex;

// ! NOTE: getDirective returns NULL if the directive does not exist

int QUIT = 0;
int	STATUS_CODE = 0;

void	getErrorPage(std::string& response, Response* res) {
	bool			inSection = false;
	std::ifstream	file;
	std::string		line;
	std::string		errorPage = getCurrentDir() + res->getErrorPage4xx();

	std::cout << "ERROR PAGE: " << errorPage << std::endl;

	if (res->getAvailableErrorCodes().count(STATUS_CODE) > 0) {
		file.open(errorPage.c_str());
		if (!file.is_open()) {
			std::cerr << "Error page not found\n";
		}
		while (getline(file, line)) {
        // Check for the start of the relevant section
			if (line.find("<!-- START " + int_to_string(STATUS_CODE) + " -->") != std::string::npos) {
				inSection = true;
				continue;
			}
			// Check for the end of the relevant section
			if (line.find("<!-- END " + int_to_string(STATUS_CODE) + " -->") != std::string::npos) {
				inSection = false;
				break;
			}
			// Add lines within the relevant section to the response
			if (inSection)
				response += line + "\n";
		}
	}

	if (response.empty())
		response = "<h1>Error Page Not Found</h1>";
}

void	readHtml(std::string &response, Request* req, Response* res) {
	std::ifstream	file;
	std::string		line;

	std::cout << "\nURL PATH: " << req->getUrlPath() << std::endl;
	std::cout << "\nPATH FOR HTML FILE: " << res->getPathForHtml() << std::endl;

	if (req->getUrlPath().substr(0, 9) == "/cgi-bin/" && req->getUrlPath() != "/cgi-bin/")
	{
		if (cgiHandler(req, STATUS_CODE, response, res) == 404) {
			getErrorPage(response, res);
			return;
		}
		file.open(req->getCgiOutput().c_str());
		if (!file.is_open()) {
			STATUS_CODE = 404;
			getErrorPage(response, res);
			return;
		}
	}
	else 
		file.open(res->getPathForHtml().c_str());
	if (!file.is_open()) {
		STATUS_CODE = 404;
		getErrorPage(response, res);
		return;
	}
	while (getline(file, line, '\0')) {
		line.insert(line.size() - 2, "\r");
		response += line;
	}
}

std::string	generateDirectoryListing(std::string urlPath, std::string root) {
	std::string newRoot;
	std::string requestPath;

	if (root.find(urlPath) != std::string::npos) {
		newRoot = root.substr(0, root.find(urlPath));
		requestPath = getCurrentDir() + newRoot + urlPath;
	} else 
		requestPath = getCurrentDir() + root + urlPath;
	DIR *dir = opendir(requestPath.c_str());
	if (!dir) {
		std::ostringstream response;
        response << "HTTP/1.1 403 Forbidden\r\n";
        response << "Content-Type: text/html\r\n";
        std::string body = "<html><head><title>403 Forbidden</title></head>"
                           "<body><h1>403 Forbidden</h1></body></html>";
        response << "Content-Length: " << body.size() << "\r\n\r\n";
        response << body;
        return response.str();
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

	STATUS_CODE = 200;
	return response.str();
}

	// * RETURN VALUES: 0 -> error, 1 -> success, 2 -> directory listing, 4 -> error 4xx, 5 -> error 5xx //
int handleGet(Request* req, Http* http, Response* res) {
	bool	locationExists = true;
	std::string	serverName = http->getServerName(req->getHost());
	std::string	locationName = http->getLocationName(req->getUrlPath(), serverName);
	if (serverName.empty())
		return 0;
	if (locationName.empty())
		locationExists = false;

	std::string cwd = getCurrentDir();
	if (cwd.empty()) {
		STATUS_CODE = 500;
		return 0;
	}

	// * Check if location exists and set all the necessary values

	setAllValues(res, http, serverName, locationName, locationExists);

	// *  Directory exists

	// TODO: double check if the function below works as expected;
	if (isADirectory(req->getUrlPath(), res->getRoot())) {
		// handle directory if location and index exist,
		if (locationExists && locationMatches(req->getUrlPath(), res->getLocationPath()) && !res->getIndex().empty()) {
			res->setPathForHtml(std::string(cwd) + res->getRoot() + "/" + res->getIndex());
		} // handle directory if location and autoindex exist
		else if (locationExists && locationMatches(req->getUrlPath(), res->getLocationPath()) && res->getAutoindex() == true) {
			return 2;
		} else if (locationExists && locationMatches(req->getUrlPath(), res->getLocationPath()) && !res->getAutoindex()) {
			STATUS_CODE = 403;
			return 4;
		} else {
			if (!res->getIndex().empty()) {
				res->setPathForHtml(std::string(cwd) + res->getRoot() + "/" + res->getIndex());
			} else if (res->getAutoindex() == true) {
				return 2;
			} else {
				STATUS_CODE = 403;
				return 4;
			}
		}
	}
	else // handle file
		res->setPathForHtml(std::string(cwd) + res->getRoot() + req->getUrlPath());

	STATUS_CODE = 200;
	return 1;
}

int handleRequest(Request* request, Http* http, Response* res) {
	if (request->getMethod() == "GET") {
		int ret = handleGet(request, http, res);
		std::cout << "RETURN VALUE: " << ret << std::endl;
		if (ret != 1)
			return ret;
		return 200;
	} else if (request->getMethod() == "POST") {
		// TODO: handle max body size check.
		// if (!handlePost(request))
			return 404;
	} else if (request->getMethod() == "DELETE") {
		// if (!handleDelete(request))
			return 404;
	} else {
		return 405;
	}
}

std::string    generateResponse(Request* req, Response* res) {
	std::string response;
	std::ostringstream oss;
	std::string index;

	switch (STATUS_CODE) {
		case 200:
			response = 
				"HTTP/1.1 200 OK\r\n"
				"Content-Type: text/html\r\n"
				"Content-Length: ";
			break;
		case 404:
			response = 
				"HTTP/1.1 404 Not Found\r\n"
				"Content-Type: text/html\r\n"
				"Content-Length: ";
			break;
		case 403:
			response = 
				"HTTP/1.1 403 Forbidden\r\n"
				"Content-Type: text/html\r\n"
				"Content-Length: ";
			break;
		case 405:
			response = 
				"HTTP/1.1 405 Method Not Allowed\r\n"
				"Content-Type: text/html\r\n"
				"Content-Length: ";
			break;
		case 500:
			response = 
				"HTTP/1.1 500 Internal Server Error\r\n"
				"Content-Type: text/html\r\n"
				"Content-Length: ";
			break;
		default:
			break;
	}
	if (STATUS_CODE == 200)
		readHtml(index, req, res);
	else
		getErrorPage(index, res);
	oss << index.length();
	response += oss.str();
	response += "\r\n\r\n";
	response += index;

	return response;
}

void	lookForStatusCode(Request* req, Http* http, Response* res) {
	std::string response = "";
	int returnCode = handleRequest(req, http, res);


	// ! NOW THAT YOU HAVE THE ERROR PAGES AND CODES, YOU CAN USE THEM TO GENERATE THE RESPONSE.
	// ! YOU CAN USE THE STATUS_CODE VARIABLE TO CHECK THE STATUS CODE OF THE RESPONSE.
	// * STATUS_CODE = 200 -> OK

	if (returnCode == 2)
		res->setResponse(generateDirectoryListing(req->getUrlPath(), res->getRoot()));
	else
		res->setResponse(generateResponse(req, res));
	std::cout << "STATUS CODE: " << STATUS_CODE << std::endl;
}

void	clientHandler(int clientSocket, Http* http) {
	char buffer[1024] = {0};
	Request *request = new Request();
	Response *res = new Response();
	std::string response;

	// Receive data from the client
	int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (bytesReceived <= 0) {
		if (bytesReceived == 0) {
			std::cout << "Client disconnected." << std::endl;
		} else {
			std::cerr << "Error receiving data." << std::endl;
		}
		close(clientSocket); // Close the socket on error or disconnect
		return;
	}
	// Print the received message
	request->parseRequest(buffer);

	// Optionally, send a response back to the client
	request->setClientId(clientSocket);
	lookForStatusCode(request, http, res);
	delete request;

	// Send the response to the client
	send(clientSocket, res->getResponse().c_str(), res->getResponse().size(), MSG_CONFIRM); // needs a check with throw error
	delete res;
}

void sigHandler(int signal) {
	if (signal == SIGINT) {
		std::cout << std::endl;
		unlinkCgi();
		QUIT = 1;
	}
}

int checkSocketAvailable(std::vector<pollfd> pollFds, int serverN) {
	for (int i = 0; i < serverN; i++) {
		if (pollFds[i].revents & POLLIN)
			return i;
	}
	return -1;
}

int main(int ac, char **av) {
	(void)ac;
	std::stringstream fileStream;
	Http *http = new Http();

	try {
		startParsing(av[1], fileStream);
		http->parse(fileStream);
	} catch (std::exception& e) {
		std::cerr << e.what();
		return 0;
	}
	// Create the server socket
	//todo thow an error on init scoket
	int serverN = 2;
	std::vector<int> serverSocket = initSocket(serverN);

	// Set socket option to allow reuse of address/port
	socketOption(serverSocket, serverN, 1);

	// Specify the address
	std::vector<sockaddr_in> serverAddress;
	if (runSocket(serverAddress, serverSocket, http, serverN) == -1)
		return 0;

	std::cout << "Server listening on port " << ntohs(serverAddress[0].sin_port) << "..." << std::endl;

	// Prepare for polling
	std::vector<pollfd> pollFds;
	for (int i = 0; i < serverN; i++) {
		pollfd serverPollFd = {serverSocket[i], POLLIN, 0};
		pollFds.push_back(serverPollFd);
	}
	signal(SIGINT, sigHandler);
	while (true) {

    // Use poll to wait for incoming connections or data
    int pollResult = poll(pollFds.data(), pollFds.size(), 200); // Timeout of 200ms
    if (pollResult < 0) {
        std::cerr << "Error with poll: " << strerror(errno) << std::endl;
        break;
    }

    // DEBUG: Log poll results

    // Handle new connections on all server sockets
    for (int i = 0; i < serverN; i++) {
        if (pollFds[i].revents & POLLIN) { // Check if the server socket is ready
            int acceptSocket = serverSocket[i];
            int clientSocket = accept(acceptSocket, NULL, NULL);
            if (clientSocket < 0) {
                std::cerr << "Client connection failed on socket " << i 
                          << " (errno: " << strerror(errno) << ")." << std::endl;
                continue;
            }
            std::cout << "Client connected on port " << ntohs(serverAddress[i].sin_port) << "." << std::endl;

            // Set the client socket to non-blocking mode
            if (setNonBlocking(clientSocket) == -1) {
                std::cerr << "Failed to set client socket to non-blocking. Closing socket." << std::endl;
                close(clientSocket);
                continue;
            }

            // Add the new client socket to the poll list
            pollfd clientPollFd = {clientSocket, POLLIN, 0};
            pollFds.push_back(clientPollFd);
        }
    }

    // Handle data from connected clients
    for (size_t i = serverN; i < pollFds.size(); i++) {
        if (pollFds[i].revents & POLLIN || pollFds[i].revents & POLLOUT) {
            // DEBUG: Log activity
            std::cout << "Handling client socket: " << pollFds[i].fd << std::endl;

            clientHandler(pollFds[i].fd, http);

            // Optionally, handle disconnection or cleanup
            if (QUIT) {
                std::cout << "QUIT signal received. Exiting loop." << std::endl;
                break;
            }
        }
    }

    // Exit outer loop on QUIT
    if (QUIT) break;
}

// Cleanup
std::cout << "pollfds = " << pollFds.size() << std::endl;
for (size_t i = 0; i < pollFds.size(); i++) {
    close(pollFds[i].fd);
}

	std::cout << "pollfds = " << pollFds.size() << std::endl;
	for (size_t i = 0; i < pollFds.size(); i++) {
		close(pollFds[i].fd);
	}
	delete http;
	return 0;
}
