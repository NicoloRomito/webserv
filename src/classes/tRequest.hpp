#ifndef TREQUEST_HPP
#define TREQUEST_HPP

#include "../../include/includes.hpp"

class TRequest {
	public:
		std::string getMethod();
		std::string getCotentLength();
		std::string getUri();
		std::string getQuery();
		std::string* getUri();
}

#endif