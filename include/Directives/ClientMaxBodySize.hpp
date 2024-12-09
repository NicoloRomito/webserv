#pragma once

#include "config.hpp"

class	ClientMaxBodySize : public AConfig {
	private:
		size_t	_bodySize;

	public:
		ClientMaxBodySize();
		ClientMaxBodySize(const std::vector<std::string>& args);
		~ClientMaxBodySize();

		const size_t&	getSize() const;

		void	parseDirective(const std::vector<std::string>& args);
};