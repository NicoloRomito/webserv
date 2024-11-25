#include "../../include/includes.hpp"
#include "../../include/Errors.hpp"

void Errors::error(std::string msg) {
	std::cerr << "[Webserver LINE: " << __LINE__ << "] -> Error: " << msg << '\n'; 
}

const char * Errors::UnknownDirectiveException::what() const throw() {
	return "Unknown directive\n";
}

const char * Errors::TooFewArgsException::what() const throw() {
	return "Too few arguments\n";
}