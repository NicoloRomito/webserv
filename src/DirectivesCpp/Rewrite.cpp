#include "../../include/includes.hpp" // IWYU pragma: keep
#include "../../include/includeClasses.hpp" // IWYU pragma: keep
#include "../../include/Errors.hpp" // IWYU pragma: keep

Rewrite::Rewrite() {}

Rewrite::~Rewrite() {}

void	Rewrite::parseDirective(std::vector<std::string> args) {
	if (args.size() != 3)
		throw Errors::TooFewArgsException("Invalid number of arguments for rewrite directive", __LINE__, __FILE__);
	_toReplace = args[0];
	_replacement = args[1];
	if (args[2].find(";") == std::string::npos)
		throw Errors::NoSemiColonException("No semicolon found", __LINE__, __FILE__);
	std::string flag = std::string(args[2].begin(), args[2].end() - 1);
	if (flag != "redirect" && flag != "permanent")
		throw Errors::InvalidArgumentException("Invalid flag for rewrite directive", __LINE__, __FILE__);
	_flag = flag;
}

const std::string& 	Rewrite::getToReplace() const {
	return _toReplace;
}

const std::string& 	Rewrite::getReplacement() const {
	return _replacement;
}

const std::string& 	Rewrite::getFlag() const {
	return _flag;
}