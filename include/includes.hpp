#ifndef INCLUDES_HPP
# define INCLUDES_HPP

#include <algorithm>
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
#include <dirent.h>
#include <iterator>
#include "parseUpload.hpp"

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[1;34m"
# define WHITE "\033[1;37m"
# define RESET "\033[0m"

//Size in Bytes
#define MEGABY * 1000000
#define GIGABY * 1000000000

// GLOBAL VARIABLES
extern int	ConfigLine;
extern int	locationN;
extern int	serverN;
extern int 	QUIT;

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
class Client;
class Upload;

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
	ALLOW_METHODS,
	REWRITE,
	UNKNOWN
};

// ReadFIles
void			getErrorPage(std::string& response, Response* res, int statusCode);
void			readHtml(std::string &response, Request* req, Response* res, int& statusCode);
void			readFile(std::string& index, Response* res, int& statusCode);

// ClientHandling
void			clientHandler(int& clientSocket, Http* http, std::string currServer);
void			lookForRequestType(Request* req, Http* http, Response* res, bool& locationExists);
std::string		generateResponse(Request* req, Response* res);
std::string		generateDirectoryListing(const std::string& urlPath, const std::string& root, Response* res);

// utilsForClientHandling
void				setMessageandRedirect(std::string& message, int statusCode, bool& isRedirect);
const std::string	setContentType(Request* req);

// RequestHandler
void			handleRequest(Request* request, Response* res, bool locationExists, int& statusCode, Upload *up);
void			handleGet(Request* req, Response* res, bool locationExists, int& statusCode);
void			handleDelete(Request* req, Response* res, int& statusCode);

// postHandler
bool 			isValidPostReq(int statusCode, Request* req);
std::string 	trimQuotes(std::string target);
std::string 	trimSpaces(std::string target);

// parseBody
void			setRequestBody(Request* req, int & statusCode);

// postReqErrors
bool 			checkPostReqErrors(Request* req, Response* res, int& statusCode);
bool 			checkforPostEntryErrors(Request* req, Response* res, int& statusCode);
bool 			checkForCgiBodyErrors(Request* req, Response* res, int& statusCode);
void 			formatError(int &statusCode, std::string encoding);

// printUtils
void			printLog(std::string msg);
void			printError(std::string msg);

// utils
DirectiveType	checkDirectiveType(const std::string& dir);
bool			isNumber(const std::string& str);
bool			checkLine(const std::string& line);
bool			semicolonFound(const std::string& line);
std::string 	int_to_string(int value);
std::string 	to_string(char value);
std::string		getCurrentDir();
bool			isADirectory(const std::string urlPath, const std::string& root);
bool			locationMatches(const std::string& urlPath, const std::string& locationPath);
void			setAllValues(Response* res, Http* http, const std::string& serverName, const std::string& locationName, bool locationExists);
void			printDebug(char symbol = '-', std::string content = "", std::string what = "", bool bottomLine = true);

// parsing
void						startParsing(const std::string& file, std::stringstream& fileStream);
std::string					parseDirective(const std::string& directive);
DirectiveType				getDirectiveType(const std::string& dir);
std::vector<std::string>	returnLine(const std::string& line);

//init socket
std::vector<int>		initSocket(int serverN);
int				setNonBlocking(int socket);
void			socketOption(std::vector<int> serverSocket, int serverN, int opt = 1);
int				runSocket(std::vector<sockaddr_in> &serverAddress, std::vector<int> serverSocket, Http* http);

#endif