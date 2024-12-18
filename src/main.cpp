#include "cgi/Cgi.hpp"
#include "classes/headers/Request.hpp"
#include <csignal>
#include <ostream>
#include <sstream>
#include <sys/poll.h>
#include <cstring>
#include <iostream>
#include "../include/includes.hpp"
#include "../include/Directives/Http.hpp"
#include "../include/Directives/Server.hpp"
#include "../include/Directives/Location.hpp"
#include "../include/Directives/ServerName.hpp"
#include "../include/Directives/Root.hpp"
#include "../include/Directives/Index.hpp"
#include "../include/Directives/ErrorPage.hpp"
#include "../include/Directives/ClientMaxBodySize.hpp"
#include "../include/Directives/Autoindex.hpp"
#include "../include/Directives/CgiPass.hpp"
#include "../include/Directives/config.hpp"
#include "../include/Directives/Listen.hpp"
#include <exception>
#include <fstream>
#include <string>
#include <vector>

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

void readIndex(std::string &response, Request* req) {
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
        file.open("src/www/static/index.html");
    if (!file.is_open()) {
        std::cout << "not open\n";
    }
    while (getline(file, line, '\0')) {
        line.insert(line.size() - 2, "\r");
        response += line;
    }
}

// std::string HandleRequest(Request* request) {

// }

std::string getResponse(Request* req) {
    std::ostringstream oss;
    std::string index;
    std::string response = "";

    if (req->getMethod() == "GET")
    {
		std::cout << "\n" << req->getPath() << "\n\n";
        // handleRequest(req);
        response = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: ";
        readIndex(index, req);
        oss << index.length();
        response += oss.str();
        response += "\r\n\r\n";
        response += index;
    } else if (req->getMethod() == "POST") {
        response = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: ";
        readIndex(index, req);
        oss << index.length();
        response += oss.str();
        response += "\r\n\r\n";
        response += index;
    } else if (req->getMethod() == "DELETE") {
        response = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: ";
        readIndex(index, req);
        oss << index.length();
        response += oss.str();
        response += "\r\n\r\n";
        response += index;
    } else {
        response = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
    }

    return response;
}

void clientHandler(int clientSocket) {
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
    std::string method = request->getMethod();
    // Optionally, send a response back to the client
    response = getResponse(request);
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
                clientHandler(pollFds[i].fd);
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
