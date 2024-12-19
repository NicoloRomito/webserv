#pragma once

#include "config.hpp"
#include <vector>

class	ErrorPage : public AConfig {
	private:
		std::vector<int>	_codes;
		std::string			_path;

	public:
		ErrorPage(int errorCode);
		ErrorPage();
		~ErrorPage();

		const std::string&	getPath() const;
		const std::vector<int>&	getCodes() const;

		void				parseDirective(const std::vector<std::string>& args);
		void				addErrorCode(const std::string& code);
};