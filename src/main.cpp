#include "../include/includes.hpp" // IWYU pragma: keep
#include <csignal>
#include <sys/poll.h>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <iostream>

int QUIT = 0;

void clientHandler(int clientSocket) {
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
        return; 
    }

    // Print the received message
    std::cout << "Message from client: " << buffer << std::endl;

    // Optionally, send a response back to the client
    const char* response = "Message received\n";
    send(clientSocket, response, strlen(response), 0);
}

void sigHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << std::endl;
        QUIT = 1;
    }
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

    signal(SIGINT, sigHandler);
    while (true) {
        // Use poll to wait for incoming connections or data
        int pollResult = poll(pollFds.data(), pollFds.size(), -1); // -1 to block indefinitely
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
            
            // Add the new client socket to the poll list
            pollfd clientPollFd = {clientSocket, POLLIN, 0};
            pollFds.push_back(clientPollFd);
        }

        // Check each client socket for incoming data
        for (size_t i = 1; i < pollFds.size(); i++) {
            if (pollFds[i].revents & POLLIN) {
                clientHandler(pollFds[i].fd);
                if (QUIT) break;
                // Don't remove client socket from poll list; keep it for further communication
                // No socket closing after receiving one message
            }
        }
    }
    std::cout << "pollfds = " << pollFds.size() << std::endl;
    for (size_t i = 0; i < pollFds.size(); i++) {
        close(pollFds[i].fd);
    }
    return 0;
}
