#pragma once

#include "config.hpp"
#include "Location.hpp"
#include <vector>


class	CgiPass : public AConfig {
	private:
		std::vector<std::string> _cgiPaths;

	public:
		CgiPass();
		CgiPass(const std::vector<std::string>& args);
		~CgiPass();

		const std::vector<std::string>&	getPath() const;
};