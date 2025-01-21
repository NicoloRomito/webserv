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
	std::string		errorPage = assignErrorPage(res, statusCode);

	std::cout << "ERROR PAGE: " << errorPage << std::endl;
	std::cout << "STATUS CODE: " << statusCode << std::endl;
	
	if (res->getAvailableErrorCodes().count(statusCode) > 0) {
		file.open(errorPage.c_str());
		if (!file.is_open()) {
			std::cerr << "Error page not found\n";
		}
		while (getline(file, line)) {
			if (line.find("<!-- START " + int_to_string(statusCode) + " -->") != std::string::npos) {
				inSection = true;
				continue;
			}
			if (line.find("<!-- END " + int_to_string(statusCode) + " -->") != std::string::npos) {
				inSection = false;
				break;
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

	std::cout << "\nURL PATH: " << req->getUrlPath() << std::endl;
	std::cout << "\nPATH FOR HTML FILE: " << res->getPathForHtml() << std::endl;

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