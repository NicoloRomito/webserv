#include "../headers/Client.hpp"
#include "../../../include/includes.hpp"
#include "../../../include/includeClasses.hpp"
#include <cstddef>
#include <string>
#include <unistd.h>

Client::Client() : _bytesRecived(0), _contentLength(0) {}

Client::~Client() {}

int	Client::readHeader(int& clientSocket)
{
	while (1) {
		_bytesRecived = recv(clientSocket, _buffer, 1, 0);
		if (_bytesRecived <= 0) {
			if (_bytesRecived == 0)
				printLog("Client disconnected.");
			else
				printError("Error receiving data.");
			close(clientSocket);
			clientSocket = -1;
			usleep(50);
			return -1;
		}
		_header += _buffer[0];
		if (_header.find("\r\n\r\n") != std::string::npos)
			break;
	}
	return 1;
}

int	Client::readBody(bool isMultipart, int& clientSocket) {
	size_t totReceived = 0;
	std::vector<char> body(_contentLength);
	int cycle = 0;

	while (totReceived < _contentLength) {
    	int bytes_received = recv(clientSocket, body.data() + totReceived, _contentLength - totReceived, MSG_WAITALL);
		if (bytes_received <= 0) {
			if (isMultipart)
			{
				if (bytes_received == 0)
					printLog("Client disconnected.");
				else if (cycle < 1 || cycle > 40000 || totReceived >= _contentLength)  //hard cap ~8mb
					printError("Error receiving data.");
			}
			else 
			{
				if (bytes_received == 0)
					printLog("Client disconnected.");
				else
					printError("Error receiving data.");
			}
			close(clientSocket);
			clientSocket = -1;
			return -1;
    	}
		if (bytes_received != -1)
    		totReceived += bytes_received;
		cycle++;
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
