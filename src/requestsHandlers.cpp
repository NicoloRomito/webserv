#include "../include/includes.hpp"
#include "../include/includeClasses.hpp"
#include "../src/classes/headers/Request.hpp"
#include "../src/classes/headers/Response.hpp"
#include "../src/classes/headers/PostFile.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <dirent.h>


void formatError(int &statusCode, std::string encoding) {
	error((encoding == "json") ?  "Invalid JSON format" : "Invalid form-urlencoded format");
	statusCode = 400;
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
	int start = 0, end = target.length() - 1; 

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
	std::string title = trimQuotes(req->getBody().at("\"filename\"")) + ".txt";
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
		"newEntry", "\"" + trimQuotes(req->getBody().at("\"filebody\"")) + "\""
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
		if (req->getQuery()[0] != '{' || req->getQuery()[req->getQuery().length() - 1] != '}' || req->getQuery().find(",") == std::string::npos)
			return formatError(statusCode, "json");
		holder = query.substr(query.find("\""), std::string::npos) + "\0";
		while (holder != "")
		{
			tempKey = holder.substr(0, holder.find("\"", 1) + 1);
			if (holder[holder.find(tempKey) + tempKey.length()] != ':')
				return formatError(statusCode, "json");
			holder = holder.substr(holder.find(tempKey) + tempKey.length(), std::string::npos);
			tempValue = holder.substr(1, holder.find(","));
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
			tempKey = holder.substr(0, holder.find("="));
			holder = holder.substr(holder.find(tempKey) + tempKey.length(), std::string::npos);
			if (holder[0] != '=' && holder[0] != '\0')
				return formatError(statusCode, "form");
			tempKey = "\"" + tempKey + "\"";
			tempValue = holder.substr(1, holder.find("&") -1);
			holder = holder.substr(holder.find(tempValue) + tempValue.length() - 1, std::string::npos);
			bodyMap.insert(std::pair<std::string, std::string>(tempKey, tempValue));
		}
	}
	req->setBody(bodyMap);
}


void	handlePost(Request* req, Response* res, int & statusCode) {

	std::string filePath;
	std::string title, body;

	if (!req->isKeyInMap("Content-Type", req->getHeader()) ||
		(req->getHeader("Content-Type") != " application/json" &&req->getHeader("Content-Type") != " application/x-www-form-urlencoded"))
	{
		statusCode = 500;
		res->setResponse(generateResponse(req, res));
		return;
	}

	if (req->getQuery() == "")
	{
		statusCode = 400;
		res->setResponse(generateResponse(req, res));
		return;
	}

	std::string cwd = getCurrentDir();
	if (cwd.empty()) {
		statusCode = 500;
		res->setResponse(generateResponse(req, res));
		return;
	}

	statusCode = 200;

    if (req->getUrlPath().find("/favicon.ico") != std::string::npos) {
        res->setPathForHtml(cwd + res->getRoot() + "/favicon.ico");
        res->setResponse(generateResponse(req, res));
        return;
    }

	setRequestBody(req, statusCode);
	if (statusCode != 200)
		return;
	if (req->getUrlPath().substr(0, 9) == "/entries/")
	{
		if (!req->isKeyInMap("\"filename\"", req->getBody()) ||
			!req->isKeyInMap("\"filebody\"", req->getBody()))
		{
			statusCode = 405;
			res->setResponse(generateResponse(req, res));
			std::cout << "POST request body invalid format\n";
			return ;
		}

		filePath = getCurrentDir() + "/src/www/static/entries/files/" + trimQuotes(req->getBody().at("\"filename\"")) + ".txt";
		if (access(filePath.c_str(), W_OK) != -1)
		{
			setResponseBodyForEntries(req, res, statusCode);
			PostFile(filePath, trimQuotes(req->getBody().at("\"filebody\"")));
		} else if (access(filePath.c_str(), F_OK) == -1) {
			statusCode = 201;
			setResponseBodyForEntries(req, res, statusCode);
			PostFile(filePath, trimQuotes(req->getBody().at("\"filebody\"")));
		} else if (access(filePath.c_str(), F_OK) != -1) {
			std::cout << "\nPermission denied\n";
			statusCode = 500;
			res->setResponse(generateResponse(req, res));
			return;
		}
	}

	res->setResponse(generateResponse(req, res));
	printDebug('+', res->getResponse(), "RESPONSE");
	
}