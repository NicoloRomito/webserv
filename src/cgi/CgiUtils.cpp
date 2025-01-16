#include "Cgi.hpp"
#include "../../include/includes.hpp"
#include "../../include/includeClasses.hpp"
#include "../classes/headers/Webserv.hpp"
#include "../../include/includes.hpp"

std::string buildCgiOutputPath(int idx) {
	std::string ret = ("./src/www/static/tmp/tmp_" + to_string(idx + 48) + ".html"); 
	return (ret);
}

char* getScriptAbsPath(std::string path, std::string root) 
{
	std::string ret;
	char cwd[100];

	getcwd(cwd, 100);

	ret  = std::string(cwd) + root + path;
	return (strdup(ret.c_str())); 
}

int countCharInstances(const std::string query, char delim) 
{
	int count = 0;

	for (size_t i = 0; i < query.length(); i++)
		if (query[i] == delim)
			count++;
	return count;
}

void unlinkCgi() {
	for (int i = 0; i < 16000; i++)
		unlink(buildCgiOutputPath(i).c_str());
}

char* getValueFromQuery(std::string query)
{
	if (query.find('=') != std::string::npos)
		return (strdup(query.substr(query.find('=') + 1, query.length()).c_str()));
	else
		return (strdup(""));
}

int cgiHandler(Request* req, int& statusCode, std::string& response, Response* res) {
	char* scriptPath;
	Cgi* cgi;

	scriptPath = getScriptAbsPath(req->getPath(), res->getRoot());
	statusCode = 200;

	if (access(scriptPath, F_OK) == -1)
	{
		statusCode = 404;
		getErrorPage(response, res, statusCode);
		free(scriptPath);
		return statusCode;
	}
	free(scriptPath);
	cgi = new Cgi(*req, res);
	cgi->executeCgi(res, req, statusCode);
	delete cgi;
	return statusCode;
}

void printDebug(char symbol, std::string content, std::string what, bool bottomLine)  
{
	std::cout << "\n";
	for (int i = 0; i < 30; i++)
		std::cout << symbol;
	std::cout << what;
	for (int i = 0; i < 30; i++)
		std::cout << symbol;
	std::cout << "\n" + content + "\n";
	if (bottomLine)
		for (size_t i = 0; i < 60 + what.length(); i++)
			std::cout << symbol;
	std::cout << "\n\n";
}