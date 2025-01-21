#pragma once

#include "../includes.hpp" // IWYU pragma: keep
#include "../includeClasses.hpp" // IWYU pragma: keep

class Rewrite: public AConfig {
	private:
		std::string		_toReplace;
		std::string		_replacement;
		std::string		_flag;

	public:
		Rewrite();
		~Rewrite();

		void	parseDirective(std::vector<std::string> args);
		
		
		const std::string& 	getToReplace() const;
		const std::string& 	getReplacement() const;
		const std::string& 	getFlag() const;		
};