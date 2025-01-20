#ifndef UPLOAD_HPP
#define UPLOAD_HPP

#include "../../../include/includes.hpp"

class Upload
{
private:
	std::string _status;
	std::string _operation;
	std::string _filename;
	std::string _filetype;


public:
	Upload();
	Upload(std::string status, std::string operation, std::string filename, std::string filetype);
	Upload(std::string status, std::string operation);

	//getters
	const std::string getStatus();
	const std::string getOperation();
	const std::string getFilename();
	const std::string getFiletype();

	//setters
	void setSuccess(std::string filename, std::string filetype);
	void setFailure();


};


#endif
