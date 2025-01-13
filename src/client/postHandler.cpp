#include "../../include/includes.hpp"
#include "../../include/includeClasses.hpp"
#include "../classes/headers/Request.hpp"
#include "../classes/headers/Response.hpp"
#include "../classes/headers/PostFile.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <dirent.h>


bool checkPostReqErrors(Request* req, Response* res, int& statusCode)
{
	//CONTENT-TYPE
	if (!req->isKeyInMap("Content-Type", req->getHeader()) ||
	(req->getHeader("Content-Type") != " application/json" && req->getHeader("Content-Type") != " application/x-www-form-urlencoded"))
	{
		statusCode = 501;
		res->setResponse(generateResponse(req, res));
		return true;
	}

	//EMPTY QUERY
	if (req->getQuery() == "")
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
		std::cout << "POST request body invalid format\n";
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
			std::cout << "POST request body invalid format\n";
			return true; 
		}
		oss.str(std::string());
	}
	return false;
}


void formatError(int &statusCode, std::string encoding) {
	error((encoding == "json") ?  "Invalid JSON format" : "Invalid form-urlencoded format");
	statusCode = 400;
}

bool isValidPostReq(int statusCode, Request* req) {
	return ((statusCode == 200 || statusCode == 201) && req->getMethod() == "POST");
}

std::string trimQuotes(std::string target) {
	
	int start = 0;
	int end = target.length() - 1;

	if (target[0] == '\"')
		start++;
	if (target[target.length() - 1] == '\"')
		end--; 
	
	return target.substr(start, end);
}

std::string trimSpaces(std::string target) {

	std::string newString;
	int start = 0, end = target.length(); 

	while (	(target[start] == ' ' ||
			target[start] == '\n' ||
			target[start] == '\r' ||
			target[start] == '\t') && start != end)
		start++;
	
	if (start == end)
		return ("");

	while (	target[end] == ' ' ||
			target[end] == '\n' ||
			target[end] == '\r' ||
			target[end] == '\t')
		end--;

	newString = target.substr(start, end);
	return newString;
} 


void setResponseBodyForEntries(Request* req, Response* res, int &statusCode)
{
	std::map<std::string, std::string> resBody;
	std::string title = trimQuotes(req->getBody().at("filename")) + ".txt";
	title = "\"" + title + "\"";
	std::string operation;

	operation = (statusCode == 200) ? "\"modify\"" : "\"create\""; 
	resBody.insert(std::pair<std::string, std::string>(
		"affected-files", title
	));
	resBody.insert(std::pair<std::string, std::string>(
		"operation", operation
	));
	resBody.insert(std::pair<std::string, std::string>(
		"status", "true"
	));
	resBody.insert(std::pair<std::string, std::string>(
		"newEntry", req->getBody().at("filebody")
	));
	res->setBody(resBody);
}



typedef std::map<std::string, std::string>::iterator mapIT;

void setRequestBody(Request* req, int & statusCode) {
	
	std::map<std::string, std::string> bodyMap;
	std::string query = req->getQuery();
	std::string holder, 
				tempKey, 
				tempValue;

	if (req->getHeader("Content-Type") == " application/json")
	{
		query = query + "\0";
		if (req->getQuery()[0] != '{' || req->getQuery()[req->getQuery().length() - 1] != '}' || req->getQuery().find(",") == std::string::npos)
			return formatError(statusCode, "json");
		holder = query.substr(query.find("\""), std::string::npos);
		if (holder == "")
			return formatError(statusCode, "json");
		while (holder != "")
		{
			tempKey = holder.substr(0, holder.find("\"", 1) + 1);
			if (holder[holder.find(tempKey) + tempKey.length() ] != ':')
				return formatError(statusCode, "json");
			holder = holder.substr(holder.find(tempKey) + tempKey.length(), std::string::npos);
			tempKey = trimQuotes(tempKey);
			if (holder.find(",") == std::string::npos)
				tempValue = holder.substr(1, holder.find("}") - 1);
			else
				tempValue = holder.substr(1, holder.find(",") - 1);
			tempValue = trimSpaces(tempValue);
			holder = holder.substr(holder.find(tempValue) + tempValue.length() + 1, std::string::npos);
			holder = trimSpaces(holder);
			bodyMap.insert(std::pair<std::string, std::string>(tempKey, tempValue));
		}

	} else if (req->getHeader("Content-Type") == " application/x-www-form-urlencoded")
	{
		holder = query + "\0";
		while (holder != "")
		{
			if (holder.find("=") == std::string::npos)
				return (formatError(statusCode, "form"));
			tempKey = holder.substr(0, holder.find("="));
			if (tempKey[0] == '&')
				tempKey = &tempKey[1];
			holder = holder.substr(holder.find(tempKey) + tempKey.length(), std::string::npos);
			if (holder[0] != '=' && holder[0] != '\0')
				return formatError(statusCode, "form");
			tempKey = tempKey;
			tempValue = holder.substr(1, holder.find("&") -1);
			holder = holder.substr(holder.find(tempValue) + tempValue.length(), std::string::npos);
			tempValue="\"" + tempValue + "\"";
			while (tempValue.find("+") != std::string::npos)
				tempValue[tempValue.find("+")] = ' ';
			bodyMap.insert(std::pair<std::string, std::string>(tempKey, tempValue));
		}
	}
	req->setBody(bodyMap);
}


void	handlePost(Request* req, Response* res, int & statusCode) {

	std::string filePath;

	statusCode = 200;

	if (checkPostReqErrors(req, res, statusCode))
		return ;

	setRequestBody(req, statusCode);

	if (statusCode != 200)
		return ;

	
	if (req->getUrlPath().substr(0, 8) == "/entries")
	{
		if (checkforPostEntryErrors(req, res, statusCode))
			return (res->setResponse(generateResponse(req, res)));

		filePath = getCurrentDir() + "/src/www/static/entries/files/" + trimQuotes(req->getBody().at("filename")) + ".txt";

		if (access(filePath.c_str(), W_OK) != -1)
		{
			setResponseBodyForEntries(req, res, statusCode);
			PostFile(filePath, trimQuotes(req->getBody().at("filebody")));
		} else if (access(filePath.c_str(), F_OK) == -1) {
			statusCode = 201;
			setResponseBodyForEntries(req, res, statusCode);
			PostFile(filePath, trimQuotes(req->getBody().at("filebody")));
		} else if (access(filePath.c_str(), F_OK) != -1) {
			std::cout << "\nPermission denied\n";
			statusCode = 403;
			res->setResponse(generateResponse(req, res));
			return;
		}
	} else if (	req->getUrlPath().substr(0, 9) == "/cgi-bin/" && 
				req->getUrlPath() != "/cgi-bin/") 
	{
		if (checkForCgiBodyErrors(req, res, statusCode))
			return ;
	} else {
		statusCode = 404;
	}

	res->setResponse(generateResponse(req, res));
}