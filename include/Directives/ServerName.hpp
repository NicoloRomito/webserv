#pragma once

#include "config.hpp"
#include <vector>

class	ServerName : public AConfig {
	private:
		std::vector<std::string>	_names;

	public:
		ServerName();
		~ServerName();

		const std::vector<std::string>&	getNames() const;

		void		parseDirective(const std::vector<std::string>& args);
};