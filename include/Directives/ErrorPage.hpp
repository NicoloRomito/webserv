#pragma once

#include "config.hpp"
#include <set>

class	ErrorPage : public AConfig {
	private:
		std::set<int>		_codes;
		std::string			_path;

	public:
		ErrorPage(int errorCode);
		ErrorPage();
		~ErrorPage();

		const std::string&	getPath() const;
		const std::set<int>&	getCodes() const;

		void				parseDirective(const std::vector<std::string>& args);
		void				addErrorCode(const std::string& code);
};