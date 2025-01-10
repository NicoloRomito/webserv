#include "../headers/PostFile.hpp"

PostFile::PostFile(std::string title, std::string body)
{
	this->_ofile = new std::ofstream(title.c_str(), std::ofstream::trunc);
	*this->_ofile << body;
	this->_ofile->close();
}

PostFile::~PostFile()
{
	delete this->_ofile;
}

const std::ofstream* PostFile::getOfile() {
	return this->_ofile;
}
