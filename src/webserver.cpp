#include "../include/includes.hpp"

int main()
{
    
    // creating socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error in socket creation." << std::endl;
        return 1;
    }

    // Set socket option to allow reuse of address/port
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Error in setting socket options." << std::endl;
        close(serverSocket);
        return 1;
    }


    // specifying the address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);  // Port 8080
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    // binding the socket to the address
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Binding failed." << std::endl;
        close(serverSocket);
        return 1;
    }

    // listening to the assigned socket
    if (listen(serverSocket, 5) < 0) {
        std::cerr << "Listen failed." << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Server listening on port " << serverAddress.sin_port  << "..."<< std::endl;

    while (1) {
        // accepting connection requests from clients
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket < 0) {
            std::cerr << "Client connection failed." << std::endl;
            continue;  // Continue listening for new connections
        }

        std::cout << "Client connected." << std::endl;

        // Receiving messages continuously
        while (1) {
            char buffer[1024] = {0};

            // Receive data from the client
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesReceived < 0) {
                std::cerr << "Error receiving data." << std::endl;
                break;  // Exit the loop if there is an error receiving data
            }
            else if (bytesReceived == 0) {
                std::cout << "Client disconnected." << std::endl;
                break;  // Exit the loop if the client has closed the connection
            }

            // Print the received message
            std::cout << "Message from client: " << buffer << std::endl;

            // Optionally, send a response back to the client
            const char* response = "Message received\n";
            send(clientSocket, response, strlen(response), 0);
        }

        // Close the client socket after communication ends
        close(clientSocket);
    }

    // closing the server socket (this won't be reached unless the server exits)
    close(serverSocket);

    return 0;
}
