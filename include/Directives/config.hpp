#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "../includes.hpp"
#include <string>
#include <vector>

class	AConfig
{
	protected:
		std::vector<std::string>	_tokens;
		std::map<std::string, AConfig *>	_directives;

	public:
		AConfig();
		virtual ~AConfig();

		virtual AConfig*	createDirective(const std::string& directive, std::vector<std::string> args);
		virtual AConfig*	createBlock(const std::string& directive, std::stringstream& file) = 0;

};

#endif