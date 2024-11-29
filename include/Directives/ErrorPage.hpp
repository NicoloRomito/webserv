#pragma once

#include "config.hpp"
#include <vector>

class	ErrorPage : public AConfig {
	private:
		std::vector<int>	_codes;
		std::string	_path;

	public:
		ErrorPage(int errorCode);
		ErrorPage(const std::vector<std::string>& args);
		~ErrorPage();

		const std::string&	getPath() const;

		void				addErrorCode(const std::string& code);
};