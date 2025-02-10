#include "../../include/includes.hpp"
#include "../../include/includeClasses.hpp"

AllowMethods::AllowMethods() {}

AllowMethods::~AllowMethods() {}

AllowMethods::AllowMethods(const std::string& get, const std::string& post, const std::string& del) {
	if (get == "GET")
		_methods.insert("GET");
	if (post == "POST")
		_methods.insert("POST");
	if (del == "DELETE")
		_methods.insert("DELETE");
}

void	AllowMethods::setMethods(std::vector<std::string> args) {
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++) {
		if (it->find(";") != std::string::npos)
			*it = it->substr(0, it->find(";"));
		if (*it == "GET" || *it == "POST" || *it == "DELETE")
			_methods.insert(*it);
		else
			throw std::invalid_argument("Invalid method");
	}
}

const std::set<std::string>&	AllowMethods::getMethods() const {
	return _methods;
}