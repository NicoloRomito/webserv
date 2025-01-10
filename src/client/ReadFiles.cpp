#include "../../include/includes.hpp"
#include "../../include/includeClasses.hpp"

void	getErrorPage(std::string& response, Response* res, int statusCode) {
	bool			inSection = false;
	std::ifstream	file;
	std::string		line;
	std::string		errorPage = getCurrentDir() + res->getErrorPage4xx();

	std::cout << "ERROR PAGE: " << errorPage << std::endl;
	std::cout << "STATUS CODE: " << statusCode << std::endl;

	if (res->getAvailableErrorCodes().count(statusCode) > 0) {
		file.open(errorPage.c_str());
		if (!file.is_open()) {
			std::cerr << "Error page not found\n";
		}
		while (getline(file, line)) {
        // Check for the start of the relevant section
			if (line.find("<!-- START " + int_to_string(statusCode) + " -->") != std::string::npos) {
				inSection = true;
				continue;
			}
			// Check for the end of the relevant section
			if (line.find("<!-- END " + int_to_string(statusCode) + " -->") != std::string::npos) {
				inSection = false;
				break;
			}
			// Add lines within the relevant section to the response
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

    if (req->getUrlPath().find("/favicon.ico") != std::string::npos) {
        // Open file in binary mode
        file.open(res->getPathForHtml().c_str(), std::ios::binary);
        if (!file.is_open()) {
            statusCode = 404;
            getErrorPage(response, res, statusCode);
            return;
        }
        
        // Get file size
        file.seekg(0, std::ios::end);
        std::streampos size = file.tellg();
        file.seekg(0, std::ios::beg);

        // Read the binary file
        char* buffer = new char[size];
        file.read(buffer, size);
        response.assign(buffer, size);
        delete[] buffer;
        
        return;
    }

	if (req->getUrlPath().substr(0, 9) == "/cgi-bin/" && req->getUrlPath() != "/cgi-bin/")
	{
		if (cgiHandler(req, statusCode, response, res))
			return;
		file.open(req->getCgiOutput().c_str());
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