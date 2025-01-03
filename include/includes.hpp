#ifndef INCLUDES_HPP
# define INCLUDES_HPP

#include <iostream>
#include <sys/stat.h>
#include <cstring>
#include <sys/poll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <exception>
#include <csignal>
#include <cstdlib>
#include <ostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <poll.h>
#include <sys/wait.h>

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[1;34m"
# define WHITE "\033[1;37m"
# define RESET "\033[0m"

// GLOBAL VARIABLES
extern int	ConfigLine;
extern int	locationN;
extern int	serverN;

class AConfig;
class Server;
class Listen;
class Http;
class Location;
class ServerName;
class Root;
class Index;
class ErrorPage;
class ClientMaxBodySize;
class Autoindex;
class CgiPass;

class Request;
class Response;

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
bool			semicolonFound(const std::string& line);
std::string 	int_to_string(int value);
std::string 	to_string(char value);
std::string		getCurrentDir();
bool			isADirectory(const std::string& urlPath, const std::string& root);
bool			locationMatches(const std::string& urlPath, const std::string& locationPath);
void			setAllValues(Response* res, Http* http, const std::string& serverName, const std::string& locationName, bool locationExists);

// parsing
void						startParsing(const std::string& file, std::stringstream& fileStream);
std::string					parseDirective(const std::string& directive);
DirectiveType				getDirectiveType(const std::string& dir);
std::vector<std::string>	returnLine(const std::string& line);

//init socket
int		initSocket();
int		setNonBlocking(int socket);
int		socketOption(int serverSocket, int opt = 1);
int		runSocket(sockaddr_in &serverAddress, int serverSocket, Http* http);

#endif