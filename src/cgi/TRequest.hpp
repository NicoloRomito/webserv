#ifndef TREQUEST_HPP
#define TREQUEST_HPP

#include "../../include/includes.hpp"
#include <string>

class TRequest {

	private:

	public:
		std::string getMethod();
		std::string getCotentLength();
		std::string getUri();
		std::string getQuery();
		std::string getHeader(std::string value);
};

#endif