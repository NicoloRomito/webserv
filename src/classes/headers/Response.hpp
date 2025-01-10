#pragma once

#include "../../../include/includes.hpp" // IWYU pragma: keep
#include <cstddef>
#include <string>
#include <set>
#include <vector>

class Response {
	private:
		size_t								_clientMaxBodySize;
		std::string							_response;
		std::string 						_root;
		std::string							_locationPath;	
		bool		 						_autoindex;
		std::string 						_index;
		std::string 						_errorPage4xx;
		std::string 						_errorPage5xx;
		std::string 						_pathForHtml;
		std::string 						_pathForCgiScript;
		std::vector<std::string>			_cgiPass;
		std::set<std::string>				_serverNames;
		std::set<int>						_statusCodes;
		std::map<std::string, std::string> 	_body;
		
	public:
		Response();
		~Response();

		bool 		isAvailableErrorCode(int code) const;
		void 		addServerNamesToHosts();
		std::string bodyToJson();

		// SETTERS
		void setClientMaxBodySize(size_t clientMaxBodySize);
		void setServerNames(const std::set<std::string>& serverNames);
		void setAvailableErrorCodes(const std::set<int>& clientCodes, const std::set<int>& serverCodes);
		void setCgiPass(const std::vector<std::string>& cgiPass);
		void setLocationPath(const std::string& locationPath);
		void setPathForHtml(const std::string& pathForHtml);
		void setPathForCgiScript(const std::string& pathForHtml);
		void setResponse(const std::string& response);
		void setRoot(const std::string& root);
		void setAutoindex(bool autoindex);
		void setIndex(const std::string& index);
		void setErrorPage4xx(const std::string& error_page4xx);
		void setErrorPage5xx(const std::string& error_page5xx);
		void setBody(const std::map<std::string, std::string> body);

		// GETTERS
		size_t										getClientMaxBodySize() const;
		bool										getAutoindex() const;
		const std::set<int>							getAvailableErrorCodes() const;
		const std::set<std::string>					getServerNames() const;
		const std::vector<std::string>				getCgiPass() const;
		const std::string&							getLocationPath() const;
		const std::string&							getPathForHtml() const;
		const std::string&							getPathForCgiScript() const;
		const std::string&							getResponse() const;
		const std::string&							getRoot() const;
		const std::string&							getIndex() const;
		const std::string&							getErrorPage4xx() const;
		const std::string&							getErrorPage5xx() const;
		const std::map<std::string, std::string>	getBody() const;
};