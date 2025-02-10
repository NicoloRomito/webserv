#include "../../include/includes.hpp"
#include "../../include/includeClasses.hpp"
#include <cstdio>
#include <fstream>
#include <string>

std::string	assignErrorPage(Response *res, int statusCode) {
	std::string code = int_to_string(statusCode);
	if (code[0] == '4')
		return getCurrentDir() + res->getErrorPage4xx();
	else
		return getCurrentDir() + res->getErrorPage5xx();
}	

void	getErrorPage(std::string& response, Response* res, int statusCode) {
	std::ifstream	file;
	std::string		line;
	bool			inSection = false;
	bool			errorSection = false;
	std::string		errorPage = assignErrorPage(res, statusCode);
	
	if (res->getAvailableErrorCodes().count(statusCode) > 0) {
		file.open(errorPage.c_str());
		if (!file.is_open()) {
			printError("Error opening error page");
		}
		while (getline(file, line)) {
			if (line.find("<!-- START ERRORS -->") != std::string::npos) {
				errorSection = true;
				continue;
			}
			if (line.find("<!-- END ERRORS -->") != std::string::npos) {
				errorSection = false;
				continue;
			}
			if (!errorSection)
				response += line + "\n";
			if (line.find("<!-- START " + int_to_string(statusCode) + " -->") != std::string::npos) {
				inSection = true;
				continue;
			}
			if (line.find("<!-- END " + int_to_string(statusCode) + " -->") != std::string::npos) {
				inSection = false;
				continue;
			}
			if (inSection)
				response += line + "\n";
		}
	}

	if (response.empty())
		response = "<h1>Error Page Not Found</h1>";
}

void	readHtml(std::string &response, Request* req, Response* res, int& statusCode) {
	std::ifstream	file;
	std::string		line;

	if (req->getUrlPath().substr(0, 9) == "/cgi-bin/" && req->getUrlPath() != "/cgi-bin/")
	{
		if (cgiHandler(req, statusCode, response, res) != 200)
		{
			getErrorPage(response, res, statusCode);
			return;
		}
		file.open(res->getPathForHtml().c_str());
		if (!file.is_open()) {
			statusCode = 404;
			getErrorPage(response, res, statusCode);
			return;
		}
	}
	else 
		file.open(res->getPathForHtml().c_str());
	if (!file.is_open()) {
		statusCode = 404;
		getErrorPage(response, res, statusCode);
		return;
	}
	while (getline(file, line, '\0')) {
		line.insert(line.size() - 2, "\r");
		response += line;
	}
}

void	readFile(std::string& index, Response* res, int& statusCode) {
	std::ifstream	fileToRead;
	std::string		line;

	// The path is already checked 
	fileToRead.open(res->getPathForHtml().c_str(), std::ios::binary);
	if (!fileToRead.is_open()) {
		statusCode = 404;
		getErrorPage(index, res, statusCode);
		return;
	}

	fileToRead.seekg(0, std::ios::end);
	std::streampos size = fileToRead.tellg();
	fileToRead.seekg(0, std::ios::beg);

	char* buffer = new char[size];
	fileToRead.read(buffer, size);
	index.assign(buffer, size);
	delete[] buffer;

	fileToRead.close();
}