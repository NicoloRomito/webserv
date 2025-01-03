#pragma once

#include "config.hpp"
#include <sstream>
#include <map>

class	AConfig;

class Location : public AConfig {
	private:
		std::string	_path;

	public:
		Location(const std::string& locationPath);
		~Location();

		const std::string&	getPath() const;

		void				parse(std::stringstream& file);
};