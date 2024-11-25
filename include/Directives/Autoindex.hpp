#pragma once

#include "config.hpp"

class	Autoindex : public AConfig {
	private:
		bool	_autoindex;

	public:
		Autoindex(const std::vector<std::string>& args);
		~Autoindex();

		const bool&	getAutoindex() const;
};