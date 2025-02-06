#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "../../../include/includes.hpp" // IWYU pragma: keep
#include <cstddef>
#include <string>
#include <vector>

class Client {
	private:
		int					_bytesRecived;
		// bool				_connected;
		char				_buffer[1];
		std::string			_header;
		// std::string			_clientMsg;

		std::vector<char>	_body;
		size_t				_contentLength;
		std::string			_boundary;


	public:
		Client();
		~Client();

		int		readHeader(int& clientSocket);
		int		readBody(bool isMultipart, int& clientSocket);

		// SETTERS
		void	setContentLength(int contentLength);
		void	setBoundary(std::string boundary);

		// GETTERS
		const	std::string& getHeader() const;
		const	std::vector<char>& getBody() const;
		const	std::string& getBoundary() const;
		size_t	getContentLength() const;

};

#endif