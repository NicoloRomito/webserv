#pragma once

#include "config.hpp"

class	Autoindex : public AConfig {
	private:
		bool	_autoindex;

	public:
		Autoindex();
		~Autoindex();

		const bool&	getAutoindex() const;

		void	parseDirective(const std::vector<std::string>& args);
};