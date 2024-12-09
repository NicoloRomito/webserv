#pragma once

#include "config.hpp"

class	Index : public AConfig {
	private:
		std::string	_defaultFile;

	public:
		Index();
		~Index();

		const std::string&	getPath() const;

		void				parseDirective(const std::vector<std::string>& args);
};