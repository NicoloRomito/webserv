#pragma once

#include "config.hpp"

class	ClientMaxBodySize : public AConfig {
	private:
		size_t	_bodySize;

	public:
		ClientMaxBodySize(std::vector<std::string> args);
		~ClientMaxBodySize();

		size_t	getSize() const;
};