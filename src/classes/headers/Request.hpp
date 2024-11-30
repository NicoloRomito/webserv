#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "../../../include/includes.hpp" // IWYU pragma: keep

class Request {
	private:
		std::string uri;
		std::string url;
		std::string header;
		std::string body;
		std::string method;
		std::string version;
		std::string path;
		std::string host;
	public:
		Request();
		~Request();
		void parseRequest(std::string buffer);
		std::string getUri() const;
		// std::string getUrl() const;
		// std::string getBody();
		// std::string getHeader();
		std::string getMethod() const;
		std::string getVersion() const;
		std::string getPath() const;
		// std::string getHost() const;
};

#endif