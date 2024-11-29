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
	public:
		Request();
		~Request();
		void parseRequest(std::string buffer);
		// std::string getUri();
		// std::string getUrl();
		// std::string getBody();
		// std::string getHeader();
		std::string getMethod() const;
		std::string getVersion() const;
		std::string getPath() const;
};

#endif