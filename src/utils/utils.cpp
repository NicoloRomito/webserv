#include "../../include/includes.hpp"
#include <cctype>
#include <cstddef>
#include <iostream>

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

std::string to_string(int value) {
    std::ostringstream oss;
    oss << value;

	std::string str = oss.str();
	oss.clear();

    return str;
}

std::string to_string(char value) {
    std::ostringstream oss;
    oss << value - '0';

	std::string str = oss.str();
	oss.clear();

    return str;
}

bool	semicolonFound(const std::string& line) {
	size_t i = 0;
	for (i = line.size() - 1; i > 0; i--) {
		if (std::isspace(line[i])) {
			continue;
		} else break;
	}
	if (i == line.size()) {
		if (line[i - 1] == ';')
			return true;
	} else {
		if (line[i] == ';')
			return true;
	}
	return false;
}
