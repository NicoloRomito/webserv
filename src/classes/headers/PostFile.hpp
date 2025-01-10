#ifndef POSTFILE_HPP
#define POSTFILE_HPP

#include "../../../include/includes.hpp"

class PostFile
{
	private:
		std::ofstream* 	_ofile;
		std::string 	_title;
		std::string 	_body;

	public:
		PostFile(std::string title, std::string body);
		~PostFile();

		//GETTERS
		const std::ofstream* getOfile();
};




#endif
