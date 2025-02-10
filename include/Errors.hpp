#pragma once

#include "includes.hpp"
#include <exception>
#include <string>

class	Errors {
	private:
		Errors();
		~Errors();
	
	public:
		static void	error(std::string msg, int line, const char *file);

		class	BaseException : public std::exception {
			private:
				std::string _message;
			public:
				BaseException(const std::string& message, int line, const char* file);
				BaseException(const std::string& message, const char* file);
				~BaseException() throw() {};

				virtual const char* what() const throw();
		};

		class	UnknownDirectiveException : public BaseException {
			public:
				UnknownDirectiveException(const std::string& message, int line, const char* file)
					: BaseException(message, line, file) {};
		};

		class	TooFewArgsException : public BaseException {
			public:
				TooFewArgsException(const std::string& message, int line, const char* file)
					: BaseException(message, line, file) {};
		};

		class	NoSemiColonException : public BaseException {
			public:
				NoSemiColonException(const std::string& message, int line, const char* file)
					: BaseException(message, line, file) {};
		};

		class	InvalidBodySizeException : public BaseException {
			public:
				InvalidBodySizeException(const std::string& message, int line, const char* file)
					: BaseException(message, line, file) {};
		};

		class	InvalidErrorCode : public BaseException {
			public:
				InvalidErrorCode(const std::string& message, int line, const char* file)
					: BaseException(message, line, file) {};
		};

		class	InvalidArgumentException : public BaseException {
			public:
				InvalidArgumentException(const std::string& message, int line, const char* file)
					: BaseException(message, line, file) {};
		};

		class	UnknownFileException : public BaseException {
			public:
				UnknownFileException(const std::string& message, const char* file)
					: BaseException(message, file) {};
		};

		class	DuplicateDirectiveException : public BaseException {
			public:
				DuplicateDirectiveException(const std::string& message, int line, const char* file)
					: BaseException(message, line, file) {};
		};

		class	SameListenException : public BaseException {
			public:
				SameListenException(const std::string& message, const char* file)
					: BaseException(message, file) {};
		};

		class	InvalidPathException : public BaseException {
			public:
				InvalidPathException(const std::string& message, int line, const char* file)
					: BaseException(message, line, file) {};
		};

		class	MemoryFailException : public BaseException {
			public:
				MemoryFailException(const std::string& message, const char* file)
					: BaseException(message, file) {};
		};

};