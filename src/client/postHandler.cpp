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

void	uploadResponse(Response* res, Upload* up, int& statusCode)
{
	std::map<std::string, std::string> resBody;
	std::string status = up->getStatus();

	statusCode = 500;

	resBody.insert(std::pair<std::string, std::string>(
		"operation", "\"" + up->getOperation() + "\""
	));
	resBody.insert(std::pair<std::string, std::string>(
		"status", "\"" + up->getStatus() + "\""
	));
	if (up->getStatus() == "success") 
	{
		statusCode = 201;
		resBody.insert(std::pair<std::string, std::string>(
			"fileName", "\"" + up->getFilename() + "\""
		));
		resBody.insert(std::pair<std::string, std::string>(
			"fileType", "\"" + up->getFiletype() + "\""
		));
	}	
	res->setBody(resBody);
}

void	handlePost(Request* req, Response* res, int & statusCode, Upload* up) {

	std::string filePath;

	statusCode = 200;

	if (checkPostReqErrors(req, res, statusCode))
		return ;

	setRequestBody(req, statusCode);

	if (statusCode != 200) {
		res->setResponse(generateResponse(req, res));
		return ;
	}

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
			printError("Permission denied");
			statusCode = 403;
			res->setResponse(generateResponse(req, res));
			return;
		}
	}	
	else if (req->getUrlPath().substr(0, 9) == "/cgi-bin/" && req->getUrlPath() != "/cgi-bin/")
	{
		if (checkForCgiBodyErrors(req, res, statusCode))
			return ;
	} else if (req->getUrlPath().substr(0, 8) == "/upload") {
		uploadResponse(res, up, statusCode);
	} else {
		statusCode = 404;
	}

	res->setResponse(generateResponse(req, res));
}