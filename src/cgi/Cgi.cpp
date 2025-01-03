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
	std::string url = request.getUrlPath();
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

char* getScriptAbsPath(std::string path) 
{
	std::string ret;
	char cwd[100];

	getcwd(cwd, 100);

	ret  = std::string(cwd) + "/src/www" + path;
	return (strdup(ret.c_str())); 
}

char** Cgi::getArgv() {
	return (this->_argv);
}

Cgi::~Cgi() {
}

int countCharInstances(const std::string query, char delim) 
{
	int count = 0;

	for (size_t i = 0; i < query.length(); i++)
		if (query[i] == delim)
			count++;
	return count;
}

char* getValueFromQuery(std::string query)
{
	if (query.find('=') != std::string::npos)
		return (strdup(query.substr(query.find('=') + 1, query.length()).c_str()));
	else
		return (strdup(""));
}

char**	Cgi::_createArgv(Request &req) {
	
	char** argv;
	std::string query;
	query = req.getQuery();
	if (req.getQuery() != "")
		query = &query[1];
	std::string tmp = query;
	int n = 0;
	
	if (req.getQuery().find('&') != std::string::npos)
	{
		n = countCharInstances(req.getQuery(), '&') + 1;
		argv = (char **)malloc(sizeof(char*) * (n + 2));
		if (!argv)
			throw Errors::MemoryFailException("Failed to allocate Memory in _createArgv(Request &req) (1)", "Cgi.cpp");
		argv[0] = getScriptAbsPath(req.getPath());
		argv[n + 1] = 0;
		for (int i = 1; i < n + 1; i++)
		{
			// std::cout << "+++++++++ARGV " + to_string(i + 48) + " +++++++++++++\n" << tmp << std::endl;
			argv[i] = getValueFromQuery(tmp.substr(0, tmp.find('&')));
			tmp = tmp.substr(tmp.find('&') + 1, tmp.length());
		}
	} else if (req.getQuery() != "") {
		argv = (char **)malloc(sizeof(char*) * 3);
		if (!argv)
		{
			error("failed malloc for argvp");
			throw;
		}
		argv[0] = getScriptAbsPath(req.getPath());
		argv[1] = getValueFromQuery(query.c_str());
		argv[2] = 0;
	} else {
		argv = (char **)malloc(sizeof(char*) * 2);
		if (!argv)
		{
			error("failed malloc for argvp");
			throw;
		}
		argv[0] = getScriptAbsPath(req.getPath());
		argv[1] = 0;
	}

	return argv;
}

Cgi::Cgi(Request & request) {
	std::string absFilePath;

	this->_env["AUTH_TYPE"] = "";
	this->_env["SERVER_SOFTWARE"] = "WEBSERV";
	this->_env["SERVER_NAME"] = "localhost";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["SERVER_PROTOCOL"] = "http/1.1";
	this->_env["SERVER_PORT"] = request.getHost().substr(request.getHost().find(':'), std::string::npos);
	this->_env["REQUEST_METHOD"] = request.getMethod();
	this->_cgiPath = request.getUrlPath();
	this->_env["PATH_INFO"] = ""; //request.getPath();
	this->_env["SCRIPT_NAME"] = request.getPath();
	// this->_env["CONTENT_TYPE"] = request.getHeader("Content-Type");
	// this->_env["CONTENT_LENGTH"] = request.getHeader("Content-Length");
	this->_env["QUERY_STRING"] = "";
	this->_env["REMOTE_USER"] = "";
	this->_env["PWD"] = "/www/cgi-bin";
	try
	{
		this->_argv = this->_createArgv(request);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
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
 	
	
	req->setCgiOutput("./src/tmp/tmp_" + to_string(nFile + 48) + ".html"); // template for our file.
	nFile++;

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
		for (int i = 0; this->_argv[i]; i++)
			free(this->_argv[i]);
		free(this->_argv);
	}
}

std::string buildCgiOutputPath(int idx) {
	std::string ret = ("./src/tmp/tmp_" + to_string(idx + 48) + ".html"); 
	return (ret);
}

void unlinkCgi() {
	for (int i = 0; access(buildCgiOutputPath(i).c_str(), F_OK) != -1; i++)
		unlink(buildCgiOutputPath(i).c_str());
}