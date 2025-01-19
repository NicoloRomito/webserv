#include "include/Directives/Listen.hpp"
#include "include/Directives/Server.hpp"
#include "include/includes.hpp"
#include "include/includeClasses.hpp"
#include "src/classes/headers/Webserv.hpp"

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

// void sigHandler(int signal) {
// 	if (signal == SIGINT) {
// 		std::cout << std::endl;
// 		unlinkCgi();
// 		QUIT = 1;
// 	}
// }

int checkSocketAvailable(std::vector<pollfd> pollFds, int serverN) {
	for (int i = 0; i < serverN; i++) {
		if (pollFds[i].revents & POLLIN)
			return i;
	}
	return -1;
}

int main(int ac, char **av) {
	if (ac != 2) {
		std::cerr << "Usage: ./webserv <config_file>" << std::endl;
		return 0;
	}
	std::stringstream fileStream;
	Http *http = new Http();
	Webserv	webserv;

	try {
		startParsing(av[1], fileStream);
		http->parse(fileStream);
	} catch (std::exception& e) {
		std::cerr << e.what();
		delete http;
		return 0;
	}

	webserv.init(http);
	webserv.initSocket();
	webserv.socketOption();
	webserv.runSocket();
	webserv.run();
	// Create the server socket
	//todo thow an error on init scoket
	// int serverN = 2;
	// std::vector<int> serverSocket = initSocket(serverN);

	// // Set socket option to allow reuse of address/port
	// socketOption(serverSocket, serverN, 1);

	// // Specify the address
	// std::vector<sockaddr_in> serverAddress;
	// if (runSocket(serverAddress, serverSocket, http) == -1)
	// {
	// 	delete http;
	// 	return 0;
	// }

	// std::cout << "Server listening on port " << ntohs(serverAddress[0].sin_port) << "..." << std::endl;

	// // Prepare for polling
	// std::vector<pollfd> pollFds;
	// for (int i = 0; i < 3; i++) {
	// 	pollfd serverPollFd = {serverSocket[i], POLLIN, 0};
	// 	pollFds.push_back(serverPollFd);
	// }
	// signal(SIGINT, sigHandler);
	// while (true) {
    // // Use poll to wait for incoming connections or data
	// 	int pollResult = poll(pollFds.data(), pollFds.size(), 200); // Timeout of 200ms
	// 	if (pollResult < 0) {
	// 		std::cerr << "Error with poll: " << strerror(errno) << std::endl;
	// 		break;
	// 	}
	// 	// DEBUG: Log poll results
	// 	// Handle new connections on all server sockets
	// 	for (int i = 0; i < 3; i++) {
	// 		if (pollFds[i].revents & POLLIN) { // Check if the server socket is ready
	// 			int acceptSocket = serverSocket[i];
	// 			int clientSocket = accept(acceptSocket, NULL, NULL);
	// 			if (clientSocket < 0) {
	// 				std::cerr << "Client connection failed on socket " << i 
	// 						<< " (errno: " << strerror(errno) << ")." << std::endl;
	// 				continue;
	// 			}
	// 			std::cout << "Client connected on port " << ntohs(serverAddress[i].sin_port) << "." << std::endl;

	// 			// Set the client socket to non-blocking mode
	// 			if (setNonBlocking(clientSocket) == -1) {
	// 				std::cerr << "Failed to set client socket to non-blocking. Closing socket." << std::endl;
	// 				close(clientSocket);
	// 				continue;
	// 			}

	// 			// Add the new client socket to the poll list
	// 			pollfd clientPollFd = {clientSocket, POLLIN, 0};
	// 			pollFds.push_back(clientPollFd);
	// 		}
	// 	}

	// 	// Handle data from connected clients
	// 	for (size_t i = 3; i < pollFds.size(); i++) {
	// 		if (pollFds[i].revents & POLLIN || pollFds[i].revents & POLLOUT) {
	// 			// DEBUG: Log activity
	// 			std::cout << "Handling client socket: " << pollFds[i].fd << std::endl;

	// 			clientHandler(pollFds[i].fd, http);
	// 			if (pollFds[i].fd == -1) {
	// 				pollFds.erase(pollFds.begin() + i);
	// 				i--;
	// 			}

	// 			// Optionally, handle disconnection or cleanup
	// 			if (QUIT) {
	// 				std::cout << "QUIT signal received. Exiting loop." << std::endl;
	// 				break;
	// 			}
	// 		}
	// 	}
	// 	// Exit outer loop on QUIT
	// 	if (QUIT) break;
	// }

	// // * Cleanup
	// std::cout << "pollfds = " << pollFds.size() << std::endl;
	// for (size_t i = 0; i < pollFds.size(); i++) {
	// 	if (pollFds[i].fd != -1)
	// 		close(pollFds[i].fd);
	// }

	delete http;
	return 0;
}
