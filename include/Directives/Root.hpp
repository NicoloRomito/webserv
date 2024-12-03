#pragma once

#include "config.hpp"

class	Root : public AConfig {
	private:
		std::string	_path;

	public:
		Root();
		~Root();

		const std::string&	getPath() const;

		void	parseDirective(const std::vector<std::string>& args);
};