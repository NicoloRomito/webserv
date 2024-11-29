#pragma once

#include "config.hpp"

class	Root : public AConfig {
	private:
		std::string	_path;

	public:
		Root();
		Root(const std::vector<std::string>& args);
		~Root();

		const std::string&	getPath() const;
};