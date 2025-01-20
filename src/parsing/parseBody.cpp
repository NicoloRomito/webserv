#include "../../include/includes.hpp"
#include "../classes/headers/Request.hpp"


typedef std::map<std::string, std::string>::iterator mapIT;


bool parseJsonBody(Request* req, std::string& query, int& statusCode, std::map<std::string, std::string>& bodyMap) {
	std::string holder,
				tempKey,
				tempValue; 

	query = query + "\0";
	
	if (req->getQuery()[0] != '{' || req->getQuery()[req->getQuery().length() - 1] != '}' || req->getQuery().find(",") == std::string::npos)
		return (formatError(statusCode, "json"), false);
	holder = query.substr(query.find("\""), std::string::npos);
	if (holder == "")
		return (formatError(statusCode, "json"), false);
	while (holder != "")
	{
		tempKey = holder.substr(0, holder.find("\"", 1) + 1);
		if (holder[holder.find(tempKey) + tempKey.length() ] != ':')
			return (formatError(statusCode, "json"), false);
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
	return true;
}

bool	parseUrlEncodedBody(std::string& query, int& statusCode, std::map<std::string, std::string>& bodyMap) {

	std::string holder,
			tempKey,
			tempValue; 

	holder = query + "\0";
	while (holder != "")
	{
		if (holder.find("=") == std::string::npos)
			return (formatError(statusCode, "form"), false);
		tempKey = holder.substr(0, holder.find("="));
		if (tempKey[0] == '&')
			tempKey = &tempKey[1];
		holder = holder.substr(holder.find(tempKey) + tempKey.length(), std::string::npos);
		if (holder[0] != '=' && holder[0] != '\0')
			return (formatError(statusCode, "form"), false);
		tempKey = tempKey;
		tempValue = holder.substr(1, holder.find("&") -1);
		holder = holder.substr(holder.find(tempValue) + tempValue.length(), std::string::npos);
		tempValue="\"" + tempValue + "\"";
		while (tempValue.find("+") != std::string::npos)
			tempValue[tempValue.find("+")] = ' ';
		bodyMap.insert(std::pair<std::string, std::string>(tempKey, tempValue));
	}
	return true;
}


void setRequestBody(Request* req, int & statusCode) {
	
	std::map<std::string, std::string> bodyMap;
	std::string query = req->getQuery();

	if (req->getHeader("Content-Type") == " application/json")
	{
		if (!parseJsonBody(req, query, statusCode, bodyMap))
			return;
	} else if (req->getHeader("Content-Type") == " application/x-www-form-urlencoded")
	{
		if (!parseUrlEncodedBody(query, statusCode, bodyMap))
			return;
	}
	req->setBody(bodyMap);
}