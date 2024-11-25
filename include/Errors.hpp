#pragma once

#include "includes.hpp"
#include <exception>
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

		class	TooFewArgsException : public std::exception {
			public:
				virtual const char * what() const throw();
		};

};