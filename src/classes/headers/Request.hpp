#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../../../include/includes.hpp" // IWYU pragma: keep
#include <string>

class Request
{
private:
	std::string uri;
	std::string url;
	std::map<std::string, std::string> header;
	std::map<std::string, std::string> body;
	std::string method;
	std::string version;
	std::string path;
	std::string host;
	std::string query;
	std::string _cgiOutput;
	int _clientId;

public:
	Request();
	~Request();

	// METHODS
	void parseRequest(std::string buffer);
	bool isKeyInMap(std::string key, std::map<std::string, std::string> map);

	// SETTERS
	void setCgiOutput(const std::string);
	void setClientId(const int);
	void setQuery(const std::string);
	void setBody(const std::map<std::string, std::string> _body);
	void setUrlPath(const std::string &urlPath);

	// GETTERS
	std::string getUri() const;
	const int &getClientId() const;
	const std::string &getUrlPath() const;
	const std::map<std::string, std::string> getHeader() const;
	const std::string &getHeader(const std::string &key) const;
	const std::string &getMethod() const;
	const std::string &getVersion() const;
	const std::string &getPath() const;
	const std::string &getCgiOutput() const;
	const std::string &getHost() const;
	const std::string &getQuery() const;
	const std::map<std::string, std::string> getBody() const;
};

void handlePost(Request *req, Response *res, int &statusCode, Upload* up);
bool invalidMethod(std::string str);

#endif