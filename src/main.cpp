#include "../include/includes.hpp"
#include "../include/includeClasses.hpp"

// * NOTE: This is a list of the directives that need to be called with numbers
// * at the end of the directive name. Here is the list:
// * Server -> server1, server2, server3, ...
// * If you have to call a directive inside server block, you have to call "server1root", "server1error_page4xx", "server1error_page5xx", ...
// * Location -> location1, location2, location3, ...
// * If you have to call a directive inside location block, you have to call "location1root", "location1error_page4xx", "location1error_page5xx", ...
// * Listen -> listen1, listen2, listen3, ...
// * ErrorPage -> server1error_page4xx, server1error_page5xx, ...

// ? Is it okay if the configuration map is empty and when you call getDirective it throws an exception? Because it's empty.

int QUIT = 0;

void readHtml(std::string &response, Request* req) {
	std::ifstream file;
	std::string line;
	Cgi* cgi;

	if (req->getPath().substr(0, 8) == "/cgi-bin")
	{
		cgi = new Cgi(*req);
		cgi->executeCgi(req);
		file.open(req->getCgiOutput().c_str());
		delete cgi;
	}
	else 
		file.open(req->getPath().c_str());
	if (!file.is_open()) {
		std::cout << "Html file not open\n";
	}
	while (getline(file, line, '\0')) {
		line.insert(line.size() - 2, "\r");
		response += line;
	}
}

int	getServerNumber(const std::string& host) {
	std::string serverNumber = host.substr(host.find(":") + 1, (host.size() - host.find(":")) - 1);
	return atoi(serverNumber.c_str());
}

bool handleGet(Request* req, Http* http) {
	int serverNumber = getServerNumber(req->getHost());
	(void)serverNumber;
	std::string host = req->getHost(); // ex: localhost:8080
	std::string path = req->getPath(); // /
	std::string root = http->getDirective<Server>("server1")->getDirective<Root>("root")->getPath(); // src/www/static
	std::string index = http->getDirective<Server>("server1")->getDirective<Index>("index")->getFile(); // index.html
	std::string error_page4xx = http->getDirective<Server>("server1")->getDirective<ErrorPage>("server1error_page4xx")->getPath(); // src/www/static/4xx.html
	std::string error_page5xx = http->getDirective<Server>("server1")->getDirective<ErrorPage>("server1error_page5xx")->getPath(); // src/www/static/5xx.html
	bool autoindex = http->getDirective<Server>("server1")->getDirective<Autoindex>("autoindex")->getAutoindex(); // true or false
	std::cout << "host = " << host << std::endl;
	std::cout << "path = " << path << std::endl;
	std::cout << "root = " << root << std::endl;
	std::cout << "index = " << index << std::endl;
	std::cout << "error_page4xx = " << error_page4xx << std::endl;
	std::cout << "error_page5xx = " << error_page5xx << std::endl;
	std::cout << "autoindex = " << autoindex << std::endl;
	std::string uri = req->getUri();
	std::cout << "uri = " << uri << std::endl;

	return true;
}

int handleRequest(Request* request, Http* http) {
	if (request->getMethod() == "GET") {
		if (!handleGet(request, http))
			return 404;
		return 200;
	} else if (request->getMethod() == "POST") {
		// if (!handlePost(request))
			return 404;
	} else if (request->getMethod() == "DELETE") {
		// if (!handleDelete(request))
			return 404;
	} else {
		return 405;
	}
}

std::string&    generateResponse(std::string& response, Request* req) {
	std::ostringstream oss;
	std::string index;

	response = 
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"Content-Length: ";

	readHtml(index, req);
	oss << index.length();
	response += oss.str();
	response += "\r\n\r\n";
	response += index;

	return response;
}

std::string getResponse(Request* req, Http* http) {
	std::string response = "";

	switch (handleRequest(req, http)) {
		case 404:
			response = "HTTP/1.1 404 Not Found\r\n\r\n"; // to set the 404 response (html)
			break;
		case 405:
			response = "HTTP/1.1 405 Method Not Allowed\r\n\r\n"; // to set the 405 response (html)
			break;
		case 200:
			response = generateResponse(response, req);
			break;
		default:
			response = generateResponse(response, req);
			break;
	}

	return response;
}

void clientHandler(int clientSocket, Http* http) {
	char buffer[1024] = {0};
	Request *request = new Request();
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
	response = getResponse(request, http);
	delete request;
	// Send the response to the client
	if (response.empty()) {
		response = "HTTP/1.1 404 Not Found\r\n\r\n";
	}
	send(clientSocket, response.c_str(), response.size(), MSG_CONFIRM); // needs a check with throw error
}

void sigHandler(int signal) {
	if (signal == SIGINT) {
		std::cout << std::endl;
		QUIT = 1;
	}
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
	int serverSocket = initSocket();
	if (serverSocket == -1)
		return 0;

	// Set socket option to allow reuse of address/port
	if (socketOption(serverSocket, 1) == -1) 
		return 0;

	// Specify the address
	sockaddr_in serverAddress;
	if (runSocket(serverAddress, serverSocket, http) == -1)
		return 0;

	std::cout << "Server listening on port " << ntohs(serverAddress.sin_port) << "..." << std::endl;

	// Prepare for polling
	std::vector<pollfd> pollFds;
	pollfd serverPollFd = {serverSocket, POLLIN, 0};
	pollFds.push_back(serverPollFd);

	signal(SIGINT, sigHandler);
	while (true) {
	// Use poll to wait for incoming connections or data
	int pollResult = poll(pollFds.data(), pollFds.size(), 200); // -1 to block indefinitely
	if (pollResult < 0) {
		std::cerr << "Error with poll." << std::endl;
		break;
	}
	// Check if the server socket is ready to accept a new connection
	if (pollFds[0].revents & POLLIN) {
		int clientSocket = accept(serverSocket, NULL, NULL);
		if (clientSocket < 0) {
			std::cerr << "Client connection failed." << std::endl;
			continue;
		}
		std::cout << "Client connected." << std::endl;
		if (setNonBlocking(clientSocket) == -1) {
			close(clientSocket);
			continue;
		}
		// Add the new client socket to the poll list
		pollfd clientPollFd = {clientSocket, POLLIN, 0};
		pollFds.push_back(clientPollFd);
	}

	// Check each client socket for incoming data
	for (size_t i = 1; i < pollFds.size(); i++) {
		if (pollFds[i].revents & POLLIN || pollFds[i].revents & POLLOUT) {
			clientHandler(pollFds[i].fd, http);
			if (QUIT) break;
			// Don't remove client socket from poll list; keep it for further communication
			// No socket closing after receiving one message
		}
	}
	if (QUIT) break;
	}
	std::cout << "pollfds = " << pollFds.size() << std::endl;
	for (size_t i = 0; i < pollFds.size(); i++) {
		close(pollFds[i].fd);
	}
	delete http;
	return 0;
}
