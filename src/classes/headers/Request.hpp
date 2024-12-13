#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "../../../include/includes.hpp" // IWYU pragma: keep
#include <string>

class Request {
	private:
		std::string uri;
		std::string url;
		std::map<std::string, std::string> header;
		std::map<std::string, std::string> body;
		std::string method;
		std::string version;
		std::string path;
		std::string host;
		std::string _cgiOutput;
	public:
		Request();
		~Request();
		void parseRequest(std::string buffer);
		std::string getUri() const;
		// std::string getUrl() const;
		std::string getBody(std::string key) const;
		std::string getHeader(std::string key) const;
		std::string getMethod() const;
		std::string getVersion() const;
		std::string getPath() const;
		std::string getCgiOutput() const;
		void setCgiOutput(const std::string);
		// std::string getHost() const;
};

#endif