#include "../../include/includes.hpp" // IWYU pragma: keep
#include "../../include/Directives/Server.hpp" // IWYU pragma: keep
#include "../../include/Directives/Http.hpp"
#include "../../include/Directives/Listen.hpp"
#include <netinet/in.h>
#include <sstream>
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

std::vector<int> initSocket(int serverN) {
    std::vector<int> serverSockets;
    int serverSocket;
    for (int i = 0; i < serverN; i++) {

        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            error("Failed to create socket");
            return serverSockets;
        }
        if (setNonBlocking(serverSocket) == -1) {
            return serverSockets;
        }
        serverSocket = setNonBlocking(serverSocket);
        serverSockets.push_back(serverSocket);
    }
	return serverSockets;
}
void socketOption(std::vector<int> serverSocket, int serverN, int option) {
    for (int i = 0; i < serverN; i++) {

        if (setsockopt(serverSocket[i], SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
            error("Failed to set server socket");
            close(serverSocket[i]);
        }
    }
}

int runSocket(std::vector<sockaddr_in> &serverAddress, std::vector<int> serverSocket, Http* http, int serverN) {
    int port;
    sockaddr_in curr;
    std::stringstream ss;

    for (int i = 0; i < serverN; i++) {
        ss << i + 1;
        std::cout << ss.str() << '\n';
        port = atoi(http->getDirective<Server>("server1")->getDirective<Listen>("listen" + ss.str())->getPort().c_str());
        ss.str("");
        curr.sin_family = AF_INET;
        curr.sin_port = htons(port);
        curr.sin_addr.s_addr = htonl(INADDR_ANY);  // Bind to any address

        // binding the socket to the address
        if (bind(serverSocket[i], (struct sockaddr*)&curr, sizeof(curr)) < 0) {
            error("Binding Failed");
            close(serverSocket[i]);
            return -1;
        }

        // listening to the assigned socket
        if (listen(serverSocket[i], 5) < 0) {
            error("Listen Failed");
            close(serverSocket[i]);
            return -1;
        }
        serverAddress.push_back(curr);
    }
    return 1;
}
