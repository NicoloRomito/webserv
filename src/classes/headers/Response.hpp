#pragma once

#include "../../../include/includes.hpp" // IWYU pragma: keep
#include <string>
#include <vector>

class Response {
	private:
		std::string					_response;
		std::string 				_root;
		std::string					_locationPath;	
		bool		 				_autoindex;
		std::string 				_index;
		std::string 				_errorPage4xx;
		std::string 				_errorPage5xx;
		std::string 				_pathForHtml;
		std::vector<std::string>	_cgiPass;
		std::vector<std::string>	_serverNames;
		std::vector<int>			_statusCodes;

	public:
		Response();
		~Response();

		bool isAvailableErrorCode(int code) const;

		// SETTERS
		void setServerNames(const std::vector<std::string>& serverNames);
		void setAvailableErrorCodes(const std::vector<int>& clientCodes, const std::vector<int>& serverCodes);
		void setCgiPass(const std::vector<std::string>& cgiPass);
		void setLocationPath(const std::string& locationPath);
		void setPathForHtml(const std::string& pathForHtml);
		void setResponse(const std::string& response);
		void setRoot(const std::string& root);
		void setAutoindex(bool autoindex);
		void setIndex(const std::string& index);
		void setErrorPage4xx(const std::string& error_page4xx);
		void setErrorPage5xx(const std::string& error_page5xx);

		// GETTERS
		bool							getAutoindex() const;
		const std::vector<int>			getAvailableErrorCodes() const;
		const std::vector<std::string>	getServerNames() const;
		const std::vector<std::string>	getCgiPass() const;
		const std::string&				getLocationPath() const;
		const std::string&				getPathForHtml() const;
		const std::string&				getResponse() const;
		const std::string&				getRoot() const;
		const std::string&				getIndex() const;
		const std::string&				getErrorPage4xx() const;
		const std::string&				getErrorPage5xx() const;
};