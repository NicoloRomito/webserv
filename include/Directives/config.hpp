#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "../includes.hpp"
#include "../Errors.hpp"
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
		virtual AConfig*	createBlock(const std::string& directive, std::stringstream& file);

		void				createDefaultDirectives(DirectiveType type);

		template <typename Directive>
		const Directive*		getDirective(const std::string& directiveName) const;
};

template <typename Directive>
const Directive*	AConfig::getDirective(const std::string& directiveName) const {
	std::map<std::string, AConfig *>::const_iterator it = _directives.find(directiveName);

	if (it != _directives.end()) {
		const Directive* directive = dynamic_cast<const Directive *>(it->second); 
		if (directive)
			return directive;
		throw Errors::UnknownDirectiveException("Directive found but type mismatch", __LINE__, __FILE__);
	}
	throw Errors::UnknownDirectiveException("Unknown directive", __LINE__, __FILE__);
}

#endif