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

		// METHODS
		void parseRequest(std::string buffer);

		// SETTERS
		void setCgiOutput(const std::string);

		// GETTERS
		// std::string getUrl() const;
		std::string 		getUri() const;
		const std::string&	getBody(const std::string& key) const;
		const std::string&	getHeader(const std::string& key) const;
		const std::string&	getMethod() const;
		const std::string&	getVersion() const;
		const std::string&	getPath() const;
		const std::string&	getCgiOutput() const;
		const std::string&	getHost() const;
};

#endif