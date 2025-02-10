#include "../../include/includes.hpp"
#include <sstream>
#include <string>
#include "../../include/Errors.hpp"
#include "../../include/colors.hpp" // IWYU pragma: keep

void Errors::error(std::string msg, int line, const char *file) {
	std::cerr << "[Webserver FILE: " << file << "; -- LINE: " << line << "] -> Error: " << msg << '\n'; 
}

Errors::BaseException::BaseException(const std::string& message, int line, const char* file) {
	std::ostringstream	oss;
	oss << line;
	_message = VIOLET400 "\n[WEBSERVER]-[EXCEPTION FILE CALL]: " + std::string(file) + ".\n" + "[CONFIGURATION FILE LINE]: " + oss.str() + ".\n[ERROR]: " + message + ".\n\n" RESET;
}

Errors::BaseException::BaseException(const std::string& message, const char* file) {
	std::ostringstream	oss;
	_message = ORANGE600 "\n[WEBSERVER]-[EXCEPTION FILE CALL]: " + std::string(file) + "\n[ERROR]: " + message + ".\n\n" RESET;
}

const char* Errors::BaseException::what() const throw() {
	return _message.c_str();
}