#pragma once

#include "includes.hpp"
#include <string>

class	Errors {
	private:
		Errors();
		~Errors();
	
	public:
		static void	error(std::string msg);

		class	UnknownDirectiveException : public std::exception {
			public:
				virtual const char* what() const throw();
		};

};