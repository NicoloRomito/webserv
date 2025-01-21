#pragma once

#include "../../include/includes.hpp"
#include "../../include/includeClasses.hpp"
#include "config.hpp"
#include <set>
#include <vector>


class AllowMethods: public AConfig {
	private:
		std::set<std::string>	_methods;

	public:
		AllowMethods();
		AllowMethods(const std::string& get, const std::string& post, const std::string& del);
		~AllowMethods();

		void	setMethods(std::vector<std::string> args);

		const std::set<std::string>&	getMethods() const;
};