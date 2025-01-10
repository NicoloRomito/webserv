#include "../../include/includes.hpp" // IWYU pragma: keep
#include "../../include/Directives/Server.hpp" // IWYU pragma: keep
#include "../../include/Directives/Http.hpp"
#include "../../include/Directives/Listen.hpp"
#include <cstdio>
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
    (void)serverN;
    std::vector<int> serverSockets;
    int serverSocket;
    for (int i = 0; i < 3; i++) {

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
    (void)serverN;
    for (int i = 0; i < 3; i++) {

        if (setsockopt(serverSocket[i], SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
            error("Failed to set server socket");
            close(serverSocket[i]);
        }
    }
}

int runSocket(std::vector<sockaddr_in> &serverAddress, std::vector<int> serverSocket, Http* http) {
    int port;
    sockaddr_in curr;
    std::stringstream server, sListen;

    int currSocket = 0;
    int serverN = http->getServerN();
    std::cout << "serverN = " << serverN << '\n';
    for (int i = 0; i < serverN; i++) {
        server << i + 1;
        int nListen = http->getDirective<Server>("server" + server.str())->getNumberOfListen();
        std::cout << "io sto lissenando porcodo: " << nListen << std::endl;
        for (int j = 0; j < nListen; j++) {
            sListen << j + 1;
            port = atoi(http->getDirective<Server>("server" + server.str())->getDirective<Listen>("listen" + sListen.str())->getPort().c_str());
            curr.sin_family = AF_INET;
            curr.sin_port = htons(port);
            curr.sin_addr.s_addr = htonl(INADDR_ANY);  // Bind to any address
            if (bind(serverSocket[currSocket], (struct sockaddr*)&curr, sizeof(curr)) < 0) {
                // error("Binding Failed");
                perror("porcodio");
                close(serverSocket[currSocket]);
                return -1;
            }
            std::cout << "invalid sto cazzo: " << j << std::endl;
            std::cout << "port: " << port << std::endl;

            // listening to the assigned socket
            if (listen(serverSocket[currSocket], 5) < 0) {
                error("Listen Failed");
                close(serverSocket[currSocket]);
                return -1;
            }
            sListen.str("");
            serverAddress.push_back(curr);
            currSocket++;
        }
        server.str("");
        // binding the socket to the address
    }
    return 1;
}
