#include "../../include/includes.hpp"
#include "../../include/colors.hpp"



void printLog(std::string msg) {
	std::cout << CYAN300 << "[WEBSERVER]-[LOG] ";
	std::cout << msg << std::endl;
	std::cout << RESET;
}

void printError(std::string msg) {
	std::cout << ROSE300 << "[WEBSERVER]-[ERROR] ";
	std::cout << msg << std::endl;
	std::cout << RESET;
}