#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "../Errors.hpp"
#include "../includes.hpp"
#include <map>
#include <iostream>
#include <string>
#include <vector>

class	AConfig
{
	protected:
		std::string							_dName;
		std::vector<std::string>			_tokens;
		std::map<std::string, AConfig *>	_directives;

	public:
		AConfig();
		virtual ~AConfig();

		virtual AConfig*	createDirective(const std::string& directive, std::vector<std::string> args);
		virtual AConfig*	createBlock(const std::string& directive, std::stringstream& file);
		virtual void		cleanDirectives();

		void				createDefaultDirectives(DirectiveType type);

		bool				alreadyExists(const std::string& directiveName) const;

		template <typename Directive>
		Directive*		getDirective(const std::string& directiveName) const;
};

template <typename Directive>
Directive*	AConfig::getDirective(const std::string& directiveName) const {
	std::map<std::string, AConfig *>::const_iterator it = _directives.find(directiveName);

	if (it != _directives.end()) {
		Directive* directive = dynamic_cast<Directive *>(it->second); 
		if (directive)
			return directive;
		throw Errors::UnknownDirectiveException("Directive found but type mismatch", __LINE__, __FILE__);
	}
	throw Errors::UnknownDirectiveException("Unknown directive " + directiveName, __LINE__, __FILE__);
}

#endif