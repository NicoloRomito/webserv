#include "../../include/includes.hpp" // IWYU pragma: keep

void printLog(std::string logMsg) {
	std::cout << "[WEBSERV LOG] -> " << logMsg << '\n';
}

void error(std::string msg) {
	std::cerr << "[WEBSERV ERROR] -> Error: " << msg << '\n'; 
}