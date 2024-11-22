#pragma once

#include "config.hpp"
#include "Location.hpp"
#include <vector>


class	CgiPass : public AConfig {
	private:
		std::string	_path;

	public:
		CgiPass(const std::vector<std::string>& args);
		~CgiPass();
};