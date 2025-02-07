#include "./Cgi.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iterator>
#include <string>
#include <unistd.h>

char **Cgi::getArgv()
{
	return (this->_argv);
}

Cgi::~Cgi()
{
}

char **Cgi::_createArgvPost(Request &req, Response *res)
{
	char **argv;
	std::stringstream oss;
	std::string arg;

	argv = (char **)calloc(sizeof(char *), req.getBody().size() + 2);
	if (!argv)
		throw Errors::MemoryFailException("Failed to allocate Memory in _createArgv(Request &req) (1)", "Cgi.cpp");
	argv[0] = getScriptAbsPath(req.getPath(), res->getRoot());
	argv[req.getBody().size() + 1] = 0;
	for (size_t i = 0; i < req.getBody().size(); i++)
	{
		oss << (i + 1);
		arg = req.getBody().at("param" + oss.str());
		if (arg[0] == '\"' && arg[arg.length() - 1] == '\"')
			arg = arg.substr(1, arg.length() - 2);
		argv[i + 1] = strdup(arg.c_str());
		oss.str(std::string());
	}

	return argv;
}

char **Cgi::_createArgvGet(Request &req, Response *res)
{
	char **argv;
	std::string query;
	query = req.getQuery();
	if (req.getQuery() != "")
		query = &query[1];
	std::string tmp = query;
	int n = 0;

	if (req.getQuery().find('&') != std::string::npos)
	{
		n = countCharInstances(req.getQuery(), '&') + 1;
		argv = (char **)malloc(sizeof(char *) * (n + 2));
		if (!argv)
			throw Errors::MemoryFailException("Failed to allocate Memory in _createArgv(Request &req) (1)", "Cgi.cpp");
		argv[0] = getScriptAbsPath(req.getPath(), res->getRoot());
		argv[n + 1] = 0;
		for (int i = 1; i < n + 1; i++)
		{
			argv[i] = getValueFromQuery(tmp.substr(0, tmp.find('&')));
			tmp = tmp.substr(tmp.find('&') + 1, tmp.length());
		}
	}
	else if (req.getQuery() != "")
	{
		argv = (char **)malloc(sizeof(char *) * 3);
		if (!argv)
		{
			printError("Failed malloc for argvp");
			throw;
		}
		argv[0] = getScriptAbsPath(req.getPath(), res->getRoot());
		argv[1] = getValueFromQuery(query.c_str());
		argv[2] = 0;
	}
	else
	{
		argv = (char **)malloc(sizeof(char *) * 2);
		if (!argv)
		{
			printError("Failed malloc for argvp");
			throw;
		}
		argv[0] = getScriptAbsPath(req.getPath(), res->getRoot());
		argv[1] = 0;
	}

	return argv;
}

Cgi::Cgi(Request &request, Response *res)
{
	std::string absFilePath;
	this->_cgiPath = request.getUrlPath();
	try
	{
		if (request.getMethod() == "GET")
			this->_argv = this->_createArgvGet(request, res);
		if (request.getMethod() == "POST")
			this->_argv = this->_createArgvPost(request, res);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}

const std::string Cgi::getCgiPath() const {
	return this->_cgiPath;
}

void Cgi::executeCgi(Response *res, Request *req, int &statusCode)
{
	pid_t waitReturn;
	int returnCode = 0;
	int cgiFd;
        int status;
        int timeout = 2; // Timeout in seconds
        int elapsed = 0;

	res->setPathForHtml("." + res->getRoot() + "/tmp/tmp_" + to_string(req->getClientId() + 48) + ".html");
	cgiFd = open(res->getPathForHtml().c_str(), O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
	if (cgiFd == -1)
		return;
	pid_t pid = fork();
	if (pid == -1) {
		close(cgiFd);
		return;
	} 
	else if (pid == 0)
	{
		dup2(cgiFd, STDOUT_FILENO);
		close(cgiFd);
		execve(this->_argv[0], this->_argv, __environ);
		exit(-1);
	} else {
        while (elapsed < timeout) {
            pid_t result = waitpid(pid, &status, WNOHANG);
            if (result == 0) {
                sleep(1);
                elapsed++;
            } else if (result > 0) {
				WIFEXITED(status);
				if (status)
					statusCode = 500;
                break;
            } else {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
        }
	}

	if (elapsed >= timeout)
	{
		kill(pid, SIGKILL);
		waitReturn = waitpid(pid, &returnCode, 0);
		if (waitReturn == -1)
			statusCode = 500;
	}
	close(cgiFd);
	for (int i = 0; this->_argv[i]; i++)
		free(this->_argv[i]);
	free(this->_argv);
}