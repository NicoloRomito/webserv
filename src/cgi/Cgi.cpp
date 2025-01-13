#include "./Cgi.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdio>
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
			error("failed malloc for argvp");
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
			error("failed malloc for argvp");
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

	this->_env["AUTH_TYPE"] = "";
	this->_env["SERVER_SOFTWARE"] = "WEBSERV";
	this->_env["SERVER_NAME"] = "localhost";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["SERVER_PROTOCOL"] = "http/1.1";
	this->_env["SERVER_PORT"] = request.getHost().substr(request.getHost().find(':'), std::string::npos);
	this->_env["REQUEST_METHOD"] = request.getMethod();
	this->_cgiPath = request.getUrlPath();
	this->_env["PATH_INFO"] = ""; // request.getPath();
	this->_env["SCRIPT_NAME"] = request.getPath();
	if (request.getMethod() == "POST")
	{
		if (request.isKeyInMap("Content-Type", request.getHeader()) &&
			request.isKeyInMap("Content-Type", request.getHeader()))
		{
			this->_env["CONTENT_TYPE"] = request.getHeader("Content-Type");
			this->_env["CONTENT_LENGTH"] = request.getHeader("Content-Length");
		}
	}
	this->_env["QUERY_STRING"] = "";
	this->_env["REMOTE_USER"] = "";

	this->_env["PWD"] = getCurrentDir() + res->getRoot() + "/cgi-bin/";
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

std::string Cgi::getCgiPath() { return this->_cgiPath; }

char **Cgi::createEnvp()
{
	std::vector<std::pair<std::string, std::string> > vect;
	char **env_matrix = new char *[_env.size() + 1];
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

void Cgi::executeCgi(Response *res, Request *req, int &statusCode)
{

	pid_t pid = fork();
	pid_t waitReturn;
	int returnCode;
	int cgiFd;

	res->setPathForHtml("." + res->getRoot() + "/tmp/tmp_" + to_string(req->getClientId() + 48) + ".html");

	cgiFd = open(res->getPathForHtml().c_str(), O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
	if (cgiFd == -1)
		return;
	if (pid == 0)
	{
		dup2(cgiFd, STDOUT_FILENO);
		close(cgiFd);
		this->createEnvp();
		execve(this->_argv[0], this->_argv, this->_envp);
		exit(-1);
	}
	waitReturn = wait(&returnCode);
	if (waitReturn >= 0 && WIFEXITED(returnCode))
	{
		if (WEXITSTATUS(returnCode) != 0)
			statusCode = 500;
	}
	close(cgiFd);
	for (int i = 0; this->_argv[i]; i++)
		free(this->_argv[i]);
	free(this->_argv);
}