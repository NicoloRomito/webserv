#include "../../include/includes.hpp"
#include <cctype>
#include <cstddef>

bool	checkLine(const std::string& line) {
	for (size_t i = 0; i < line.size(); i++) {
		if (line[i] == '#') {
			return false;
		}
		if (line[i] == '\t' || line[i] == ' ') {
			continue;
		}
		else {
			return true;
		}

	}
	return false;
}

bool	isNumber(const std::string& str) {
	for (size_t i = 0; i < str.size(); i++) {
		if (!std::isdigit(str[i])) {
			return false;
		}
	}
	return true;
}