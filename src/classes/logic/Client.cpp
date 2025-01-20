#include "../headers/Client.hpp"
#include "../../../include/includes.hpp"
#include "../../../include/includeClasses.hpp"
#include <cstddef>
#include <string>

Client::Client(int& socket) : _socket(socket), _bytesRecived(0), _contentLength(0) {}

Client::~Client() {}

void	Client::closeSocket() {
	close(_socket);
	_socket = -1;
}

int	Client::readHeader()
{
	while (1) {
		_bytesRecived = recv(_socket, _buffer, 1, 0);
		if (_bytesRecived <= 0) {
			if (_bytesRecived == 0)
				std::cout << "Client disconnected." << std::endl;
			else
				std::cerr << "Error receiving data\n";
			return -1;
		}
		_header += _buffer[0];
		if (_header.find("\r\n\r\n") != std::string::npos)
			break;
	}
	return 1;
}

int	Client::readBody() {
	size_t totReceived = 0;
	std::vector<char> body(_contentLength);
	while (totReceived < _contentLength) {
    	int bytes_received = recv(_socket, body.data() + totReceived, _contentLength - totReceived, 0);        	
		if (bytes_received <= 0) {
			if (bytes_received == 0)
				std::cout << "Client disconnected." << std::endl;
			else
				std::cerr << "Error receiving data\n";
			return -1;
    	}
    	totReceived += bytes_received;
    }
	_body = body;
	return 1;
}


// SETTERS //

void	Client::setContentLength(int contentLength) {
	_contentLength = contentLength;
}

// GETTERS //

const std::string&	Client::getHeader() const {
	return _header;
}

const std::vector<char>&	Client::getBody() const {
	return _body;
}

size_t	Client::getContentLength() const {
	return _contentLength;
}

int	Client::getSocket() const {
	return _socket;
}