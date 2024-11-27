#include "../../include/Directives/ErrorPage.hpp"
#include "../../include/Errors.hpp"
#include "../../include/includes.hpp"
#include <cstdlib>
#include <string>
#include <vector>

void ErrorPage::addErrorCode(const std::string& code) {
	int errorCode = atoi(code.c_str());

	if (code[0] == '4') {
		switch (errorCode) {
			case 400:
				_codes.push_back(400);
				break;
			case 403:
				_codes.push_back(403);
				break;
			case 404:
				_codes.push_back(404);
				break;
			case 405:
				_codes.push_back(405);
				break;
			case 408:
				_codes.push_back(408);
				break;
			default:
				throw Errors::InvalidErrorCode("Invalid error code", __LINE__, __FILE__);
		}
	} else if (code[0] == '5') {
		switch (errorCode) {
			case 500:
				_codes.push_back(500);
				break;
			case 502:
				_codes.push_back(502);
				break;
			default:
				throw Errors::InvalidErrorCode("Invalid error code", __LINE__, __FILE__);
		}
	}
}

ErrorPage::ErrorPage(const std::vector<std::string>& args) {
	for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); it++) {
		if (isNumber(*it)) {
			addErrorCode(*it);
		} else if (it->find(".html") != std::string::npos) {
			_path = std::string(it->begin(), it->end() - 1); // remove the last character
		} else {
			throw Errors::InvalidArgumentException("Invalid path for error page", __LINE__, __FILE__);
		}
	}
}

ErrorPage::~ErrorPage() {
	_codes.clear();
}

const std::string& ErrorPage::getPath() const {
	return _path;
}