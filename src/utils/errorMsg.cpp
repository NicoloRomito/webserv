#include "../../include/includes.hpp" // IWYU pragma: keep
#include "../../include/Errors.hpp"

void Errors::error(std::string msg) {
	std::cerr << "[Webserver LINE: " << __LINE__ << "] -> Error: " << msg << '\n'; 
}

const char * Errors::UnknownDirectiveException::what() const throw() {
	return "Unknown directive\n";
}