#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "includes.hpp"

class	Config
{
	private:
		std::map<std::string, std::string>	_directives;

	public:
		Config();
		virtual ~Config();

		void	startParsingFile(std::string file);

};

#endif