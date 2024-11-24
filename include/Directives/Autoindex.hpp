#pragma once

#include "config.hpp"

class	Autoindex : public AConfig {
	private:
		bool	_autoindex;

	public:
		Autoindex(std::vector<std::string> args);
		~Autoindex();

		bool	getAutoindex() const;
};