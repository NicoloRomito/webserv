#include "../../include/includes.hpp" // IWYU pragma: keep
#include <netinet/in.h>

int initSocket() {
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        error("failed to create socket");
        return -1;
    }
	return serverSocket;
}

int socketOption(int serverSocket, int option) {
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
        error("failed to set server socket");
        close(serverSocket);
        return -1;
    }
	return 1;
}

int runSocket(sockaddr_in &serverAddress, int serverSocket) {
	serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);  // Port 8080
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

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