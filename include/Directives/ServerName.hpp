#pragma once

#include "config.hpp"
#include <set>

class	ServerName : public AConfig {
	private:
		std::set<std::string>	_names;

	public:
		ServerName();
		~ServerName();

		const std::set<std::string>&	getNames() const;

		void		parseDirective(const std::vector<std::string>& args);
};