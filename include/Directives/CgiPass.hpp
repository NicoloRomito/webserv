#pragma once

#include "config.hpp"
#include "Location.hpp"
#include <vector>


class	CgiPass : public AConfig {
	private:
		std::vector<std::string> _cgiPaths;

	public:
		CgiPass();
		~CgiPass();

		void	setDefaultCgiPaths();

		const std::vector<std::string>&	getPath() const;

		void	parseDirective(const std::vector<std::string>& args);
};