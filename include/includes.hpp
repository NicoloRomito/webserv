#ifndef INCLUDES_HPP
# define INCLUDES_HPP

#include <iostream> // IWYU pragma: keep 
#include <cstring> // IWYU pragma: keep
#include <netinet/in.h> // IWYU pragma: keep
#include <sys/socket.h>
#include <unistd.h>
#include <map> // IWYU pragma: keep
#include <vector> // IWYU pragma: keep
#include <poll.h>

void error(std::string msg);

//init socket
int initSocket();
int socketOption(int serverSocket, int opt = 1);
int runSocket(sockaddr_in &serverAddress, int serverSocket);

#endif