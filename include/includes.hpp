#ifndef INCLUDES_HPP
# define INCLUDES_HPP

#include <iostream> 
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <poll.h>

// GLOBALE VARIABLES
extern int	ConfigLine;
extern int	locationN;
extern int	serverN;

class AConfig;

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

// utils
DirectiveType	checkDirectiveType(const std::string& dir);
bool			isNumber(const std::string& str);
bool			checkLine(const std::string& line);
std::string 	to_string(int value);

// parsing
void						startParsing(const std::string& file, std::stringstream& fileStream);
std::string					parseDirective(const std::string& directive);
DirectiveType				getDirectiveType(const std::string& dir);
std::vector<std::string>	returnLine(const std::string& line);

//init socket
int initSocket();
int socketOption(int serverSocket, int opt = 1);
int runSocket(sockaddr_in &serverAddress, int serverSocket);

#endif