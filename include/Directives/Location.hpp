#pragma once

#include "config.hpp"
#include <sstream>
#include <map>

class	AConfig;

class Location : public AConfig {
	private:
		std::string	_path;

	public:
		Location();
		~Location();

		const std::string&	getPath() const;

		void				parse(std::stringstream& file);

		const AConfig*		getDirective(const std::string& directiveName) const;
};