#include "../../include/includes.hpp" // IWYU pragma: keep

void error(std::string msg) {
	std::cerr << "[webserver LOG] -> Error: " << msg << '\n'; 
}