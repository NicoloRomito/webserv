#ifndef INCLUDES_HPP
# define INCLUDES_HPP

#include "Directives/config.hpp"
#include <iostream> // IWYU pragma: keep 
#include <cstring> // IWYU pragma: keep
#include <netinet/in.h> // IWYU pragma: keep
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <string>
#include <map> // IWYU pragma: keep
#include <vector> // IWYU pragma: keep
#include <poll.h>

// enum	ContextType {
// 	SERVER,
// 	LOCATION,
// 	UNKNOWN
// };

enum	DirectiveType {
	HTTP,
	SERVER,
	LISTEN,
	SERVER_NAME,
	ROOT,
	INDEX,
	ERROR_PAGE,
	CLIENT_MAX_BODY_SIZE,
	AUTOINDEX,
	LOCATION,
	CGI_PASS,
	UNKNOWN
};

// errors
void 	error(std::string msg);

// parsing
void						startParsing(std::string file);
AConfig*					createBlock(const std::string& directive, std::vector<std::string> args, std::stringstream& file);
std::string					parseDirective(const std::string& directive);
DirectiveType				getDirectiveType(const std::string& dir);
std::vector<std::string>	returnLine(const std::string& line);

//init socket
int initSocket();
int socketOption(int serverSocket, int opt = 1);
int runSocket(sockaddr_in &serverAddress, int serverSocket);

#endif