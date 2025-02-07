#include "../../include/includes.hpp"
#include "../classes/headers/Request.hpp"
#include "../classes/headers/Response.hpp"


bool checkPostReqErrors(Request* req, Response* res, int& statusCode)
{
	//CONTENT-TYPE
	if (!req->isKeyInMap("Content-Type", req->getHeader()) ||
	(req->getHeader("Content-Type") != " application/json" && 
	req->getHeader("Content-Type") != " application/x-www-form-urlencoded" &&
	req->getHeader("Content-Type").find("multipart/form-data;") == std::string::npos))
	{
		statusCode = 501;
		res->setResponse(generateResponse(req, res));
		return true;
	}

	//EMPTY QUERY
	if (req->getQuery() == "" && 
		req->getHeader("Content-Type").find("multipart/form-data;") == std::string::npos)
	{
		statusCode = 400;
		res->setResponse(generateResponse(req, res));
		return true;
	}

	//BODY EXCEEDS LIMIT
	if (req->getQuery().length() > res->getClientMaxBodySize())
	{
		statusCode = 413;
		res->setResponse(generateResponse(req, res));
		return true;
	}

	//EMPTY CWD
	std::string cwd = getCurrentDir();
	if (cwd.empty()) {
		statusCode = 500;
		res->setResponse(generateResponse(req, res));
		return true;
	}

	return false;
}

bool checkforPostEntryErrors(Request* req, Response* res, int& statusCode)
{
	char reqUrlLastChar;
	

	//ENTRIES ERRORS
	reqUrlLastChar = req->getUrlPath()[8];
	
	if (reqUrlLastChar != '/' && reqUrlLastChar != 0)
	{
		statusCode = 404;
		res->setResponse(generateResponse(req, res));
		return true;
	}
	if (!req->isKeyInMap("filename", req->getBody()) ||
		!req->isKeyInMap("filebody", req->getBody()) ||
		req->getBody().at("filename")[0] != '"'	||
		req->getBody().at("filebody")[0] != '"' )
	{
		statusCode = 422;
		res->setResponse(generateResponse(req, res));
		printError("POST request body invalid format");
		return true;
	}
	return false;
}

bool checkForCgiBodyErrors(Request* req, Response* res, int& statusCode)
{
	std::stringstream oss;
	for (size_t i = 0; i < req->getBody().size(); i++)
	{
		oss << (i + 1);
		if (!req->isKeyInMap("param" + oss.str(), req->getBody()))
		{
			statusCode = 422;
			res->setResponse(generateResponse(req, res));
			printError("POST request body invalid format");
			return true; 
		}
		oss.str(std::string());
	}
	return false;
}

void formatError(int &statusCode, std::string encoding) {
	printError((encoding == "json") ?  "Invalid JSON format" : "Invalid form-urlencoded format");
	statusCode = 400;
}
