#include "./Cgi.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <iterator>
#include <string>
#include <unistd.h>



std::string getScriptName(Request request) {

	std::string delimiter = "/";
	std::string url = request.getPath();
	std::string tmp;

	while (url.find(delimiter) != std::string::npos)
	{
		tmp = url.substr(0, url.find(delimiter));
		url = url.substr(url.find(delimiter) + 1, std::string::npos);
		if (tmp == "cgi-bin")
			if (url != "")
				return (url.substr(0, url.find(delimiter) - 1));
	}
	return ("");
};

std::string getPort() {
	return ("8080");
}

std::string getScriptPath(std::string path) 
{
	std::string ret;
	char cwd[100];

	getcwd(cwd, 100);

	ret  = std::string(cwd) + "/src/www" + path;
	return (ret); 
}

char** Cgi::getArgv() {
	return (this->_argv);
}

std::string getHeader(std::string key){
	if (key != "")
		return "apllication/x-python-code";
	return "";
}

std::string getCotentLength()
{
	return ("100");
}

Cgi::~Cgi() {
	free(this->_argv[0]);
	delete [] this->_argv;
}

Cgi::Cgi(Request & request) {
	std::string absFilePath;

	this->_env["AUTH_TYPE"] = "";
	this->_env["SERVER_SOFTWARE"] = "WEBSERV";
	this->_env["SERVER_NAME"] = "localhost";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["SERVER_PROTOCOL"] = "http/1.1";
	this->_env["SERVER_PORT"] = getPort();
	this->_env["REQUEST_METHOD"] = request.getMethod();
	this->_cgiPath = request.getPath();
	this->_env["PATH_INFO"] = ""; //request.getPath();
	this->_env["SCRIPT_NAME"] = request.getPath();
	this->_env["CONTENT_TYPE"] = getHeader("content-type");
	this->_env["CONTENT_LENGTH"] = getCotentLength();
	this->_env["QUERY_STRING"] = "";
	this->_env["REMOTE_USER"] = "";
	this->_env["PWD"] = "/www/cgi-bin";
	this->_argv = new char*[2];
	absFilePath = getScriptPath(request.getPath());
	std::cout << absFilePath << "\n";
	this->_argv[0] = strdup(absFilePath.c_str());
	this->_argv[1] = 0;
}

std::string Cgi::getCgiPath() {return this->_cgiPath;}

char**	Cgi::createEnvp()
{
	std::vector<std::pair<std::string, std::string> > vect;
	char** env_matrix = new char*[_env.size() + 1];
	env_matrix[_env.size()] = 0;
	std::string tmp;

	std::copy(this->_env.begin(), this->_env.end(), std::back_inserter(vect));
	
	for (size_t i = 0; i < _env.size(); i++)
	{
		tmp = (vect[i].first + "=" + vect[i].second);

		env_matrix[i] = strdup(tmp.c_str());
	}

	this->_envp = env_matrix;

	return (env_matrix);
}

void Cgi::executeCgi(Request *req) {

	int pid = fork();
	int oldStdOut = dup(STDOUT_FILENO);
	static int nFile = 0;
  	int cgiFd;
 	req->setCgiOutput("./src/tmp/tmp_" + to_string(nFile) + ".html"); // template for our file.


	cgiFd = open(req->getCgiOutput().c_str(),  O_CREAT | O_WRONLY, 0777);
	if (cgiFd == -1)
		return ;

	if (pid == 0)
	{
		dup2(cgiFd, STDOUT_FILENO);
		close(cgiFd);
		close(STDIN_FILENO);
		execve(this->_argv[0], this->_argv, createEnvp());
	}
	else {
		wait(&cgiFd);
		dup2(STDOUT_FILENO, oldStdOut);
		close(oldStdOut);
		close(cgiFd);
		// unlink(filename);
	}
}