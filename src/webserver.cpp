#include "../include/includes.hpp" // IWYU pragma: keep
#include <sys/poll.h>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <iostream>

void clientHandler(int clientSocket, bool& shouldRemove) {
    char buffer[1024] = {0};

    // Receive data from the client
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) {
        if (bytesReceived == 0) {
            std::cout << "Client disconnected." << std::endl;
        } else {
            std::cerr << "Error receiving data." << std::endl;
        }
        close(clientSocket); // Close the socket on error or disconnect
        shouldRemove = true; // Mark this client FD for removal
        return; 
    }

    // Print the received message
    std::cout << "Message from client: " << buffer << std::endl;

    // Optionally, send a response back to the client
    const char* response = "Message received\n";
    send(clientSocket, response, strlen(response), 0);
}

int main() {
    // Create the server socket
    int serverSocket = initSocket();
    if (serverSocket == -1) return 0;

    // Set socket option to allow reuse of address/port
    int opt = 1;
    socketOption(serverSocket, opt);

    // Specify the address
    sockaddr_in serverAddress;
    runSocket(serverAddress, serverSocket);

    std::cout << "Server listening on port " << ntohs(serverAddress.sin_port) << "..." << std::endl;

    // Prepare for polling
    std::vector<pollfd> pollFds;
    pollfd serverPollFd = {serverSocket, POLLIN, 0};
    pollFds.push_back(serverPollFd);

    while (true) {
        // Poll for activity
        int pollCount = poll(&pollFds[0], pollFds.size(), -1);
        if (pollCount < 0) {
            std::cerr << "Poll error." << std::endl;
            break;
        }

        // Iterate over file descriptors
        for (size_t i = 0; i < pollFds.size(); ++i) {
            if (pollFds[i].revents & POLLIN) {
                if (pollFds[i].fd == serverSocket) {
                    // Accept a new client connection
                    int clientSocket = accept(serverSocket, NULL, NULL);
                    if (clientSocket < 0) {
                        std::cerr << "Client connection failed." << std::endl;
                        continue;
                    }
                    std::cout << "Client connected." << std::endl;

                    // Add new client socket to the poll list
                    pollfd clientPollFd = {clientSocket, POLLIN, 0};
                    pollFds.push_back(clientPollFd);
                } else {
                    // Handle client data
                    bool shouldRemove = false;
                    clientHandler(pollFds[i].fd, shouldRemove);

                    // If client is disconnected, mark its FD for removal
                    if (shouldRemove) {
                        close(pollFds[i].fd); // Close the client socket
                        pollFds.erase(pollFds.begin() + i); // Remove FD from vector
                        --i; // Adjust index due to vector resize
                    }
                }
            }
        }
    }

    // Clean up server socket
    close(serverSocket);
    return 0;
}
