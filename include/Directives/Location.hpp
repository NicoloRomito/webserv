#pragma once

#include "config.hpp"
#include <sstream>
#include <map>

class	AConfig;

class Location : public AConfig {
	private:
		std::string	_path;

	public:
		Location(std::stringstream& file);
		~Location();

		const std::string&	getPath() const;
};