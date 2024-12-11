#include "../../include/includes.hpp" // IWYU pragma: keep
#include "../../include/Directives/Server.hpp" // IWYU pragma: keep
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

int initSocket() {
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        error("Failed to create socket");
        return -1;
    }
    if (setNonBlocking(serverSocket) == -1) {
        return -1;
    }
    serverSocket = setNonBlocking(serverSocket);
	return serverSocket;
}

int socketOption(int serverSocket, int option) {
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
        error("Failed to set server socket");
        close(serverSocket);
        return -1;
    }
	return 1;
}

int runSocket(sockaddr_in &serverAddress, int serverSocket, const std::string& port) {
    int Port = atoi(port.c_str());
    (void)Port;
    // /etc/hosts

	serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8081);  // Port 8080
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);  // Bind to any address

    // binding the socket to the address
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        error("Binding Failed");
        close(serverSocket);
        return -1;
    }

    // listening to the assigned socket
    if (listen(serverSocket, 5) < 0) {
		error("Listen Failed");
		close(serverSocket);
		return -1;
	}
	return 1;
}