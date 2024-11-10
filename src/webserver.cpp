#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;
int main()
{
    
    // creating socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "Error in socket creation." << endl;
        return 1;
    }

    // Set socket option to allow reuse of address/port
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        cerr << "Error in setting socket options." << endl;
        close(serverSocket);
        return 1;
    }


    // specifying the address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);  // Port 8080
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // binding the socket to the address
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cerr << "Binding failed." << endl;
        close(serverSocket);
        return 1;
    }

    // listening to the assigned socket
    if (listen(serverSocket, 5) < 0) {
        cerr << "Listen failed." << endl;
        close(serverSocket);
        return 1;
    }

    cout << "Server listening on port 8080..." << endl;

    while (1) {
        // accepting connection requests from clients
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket < 0) {
            cerr << "Client connection failed." << endl;
            continue;  // Continue listening for new connections
        }

        cout << "Client connected." << endl;

        // Receiving messages continuously
        while (1) {
            char buffer[1024] = {0};

            // Receive data from the client
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesReceived < 0) {
                cerr << "Error receiving data." << endl;
                break;  // Exit the loop if there is an error receiving data
            } 
            else if (bytesReceived == 0) {
                cout << "Client disconnected." << endl;
                break;  // Exit the loop if the client has closed the connection
            }

            // Print the received message
            cout << "Message from client: " << buffer << endl;

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
