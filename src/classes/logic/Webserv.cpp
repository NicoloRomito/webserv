#include "../headers/Webserv.hpp"
#include "../../../include/includes.hpp"
#include "../../../include/includeClasses.hpp" // IWYU pragma: keep
#include <csignal>
#include <cstdio>
#include <netinet/in.h>
#include <string>
#include <cstdlib>
#include <fcntl.h>

int    setNonBlocking(int socket) {
    int flags = fcntl(socket, F_GETFL, 0); // get the current flags of the socket
    if (flags == -1) {
        error("Failed to get flags");
        return -1;
    }
    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1) { // set the socket to non-blocking
        error("Failed to set non-blocking");
        return -1;
    }
    return socket;
}

Webserv::Webserv(): portN(0), option(1), http(NULL), pollFds(0), serverSocket(0), serverAddress(0) {}

void Webserv::init(Http *http) {
	this->http = http;
	int		serverN;
	Server	*server;

	serverN = http->getServerN();
	for (int i = 0; i < serverN; i++) {
		server = http->getDirective<Server>("server" + int_to_string(i + 1));
		for (int j = 0; j < server->getNumberOfListen(); j++) {
			this->portN++;
		}
	}
}

void Webserv::initSocket() {
	int currSocket;
    for (int i = 0; i < portN; i++) {

        currSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (currSocket < 0) {
            error("Failed to create socket");
        }
        if (setNonBlocking(currSocket) == -1) {
        }
        currSocket = setNonBlocking(currSocket);
        serverSocket.push_back(currSocket);
    }
}

void Webserv::socketOption() {
   for (int i = 0; i < portN; i++) {

        if (setsockopt(serverSocket[i], SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
            error("Failed to set server socket");
            close(serverSocket[i]);
        }
    }
}

void Webserv::runSocket() {
	int port;
    sockaddr_in curr;
    std::stringstream server, sListen;

    int currSocket = 0;
    int serverN = http->getServerN();
    for (int i = 0; i < serverN; i++) {
        server << i + 1;
        int nListen = http->getDirective<Server>("server" + server.str())->getNumberOfListen();
        for (int j = 0; j < nListen; j++) {
            sListen << j + 1;
            port = atoi(http->getDirective<Server>("server" + server.str())->getDirective<Listen>("listen" + sListen.str())->getPort().c_str());
            curr.sin_family = AF_INET;
            curr.sin_port = htons(port);
            curr.sin_addr.s_addr = htonl(INADDR_ANY);  // Bind to any address
            if (bind(serverSocket[currSocket], (struct sockaddr*)&curr, sizeof(curr)) < 0) {
                // error("Binding Failed");
                perror("Error: ");
                close(serverSocket[currSocket]);
            }
			this->listenMap[serverSocket[currSocket]] = "server" + int_to_string(i + 1);
            // listening to the assigned socket
            if (listen(serverSocket[currSocket], 1024) < 0) {
                error("Listen Failed");
                close(serverSocket[currSocket]);
            }
            sListen.str("");
            serverAddress.push_back(curr);
            currSocket++;
        }
        server.str("");
        // binding the socket to the address
    }
}

void sigHandler(int signal) {
	if (signal == SIGINT) {
		std::cout << std::endl;
		// unlinkCgi();
		QUIT = 1;
	}
}

int Webserv::acceptClient() {
	for (size_t i = portN; i < pollFds.size(); i++) {
			if (pollFds[i].revents & POLLIN || pollFds[i].revents & POLLOUT) {
				// DEBUG: Log activity
				printLog("Handling client socket: " + int_to_string(pollFds[i].fd));
				clientHandler(pollFds[i].fd, http, currServer);
				if (pollFds[i].fd == -1) {
					pollFds.erase(pollFds.begin() + i);
					i--;
				}

				// Optionally, handle disconnection or cleanup
				if (QUIT) {
					printLog("QUIT signal received. Exiting loop.");
					return 1;
				}
			}
		}
		return 1;
}

int Webserv::handleNewConnection() {
	int		acceptSocket, clientSocket;
	pollfd	clientPollFd;

	for (int i = 0; i < portN; i++) {
			if (pollFds[i].revents & POLLIN) { // Check if the server socket is ready
				acceptSocket = serverSocket[i];

				currServer = this->listenMap[acceptSocket];

				clientSocket = accept(acceptSocket, NULL, NULL);
				if (clientSocket < 0) {
					std::cerr << "Client connection failed on socket " << i 
							<< " (errno: " << strerror(errno) << ")." << std::endl;
					continue;
				}
		
				printLog("Client connected on port " + int_to_string(ntohs(serverAddress[i].sin_port)) + ".");

				// Set the client socket to non-blocking mode
				if (setNonBlocking(clientSocket) == -1) {
					std::cerr << "Failed to set client socket to non-blocking. Closing socket." << std::endl;
					close(clientSocket);
					continue;
				}

				clientPollFd.fd = clientSocket;
				clientPollFd.events = POLLIN;
				clientPollFd.revents = 0;

				pollFds.push_back(clientPollFd);
			}
		}
		return 1;
}

void Webserv::run() {
	int pollResult;

	for (size_t i = 0; i < serverAddress.size(); i++) {
		printLog("Server listening on port http://localhost:" + int_to_string(ntohs(serverAddress[i].sin_port)) + "...");
	}

	for (int i = 0; i < portN; i++) {
		pollfd serverPollFd = {serverSocket[i], POLLIN, 0};
		pollFds.push_back(serverPollFd);
	}
	signal(SIGINT, sigHandler);
	while (true) {
		pollResult = poll(pollFds.data(), pollFds.size(), 200); // Timeout of 200ms
		if (pollResult < 0 && !QUIT) {
			std::cerr << "Error with poll: " << strerror(errno) << std::endl;
			break;
		}

		if (!handleNewConnection())
			continue;

		if (!acceptClient())
			break;

		if (QUIT) break;

	}

	std::cout << "pollfds = " << pollFds.size() << std::endl;
	for (size_t i = 0; i < pollFds.size(); i++) {
		if (pollFds[i].fd != -1)
			close(pollFds[i].fd);
	}
}
