#include "../headers/Upload.hpp"


//constructors
Upload::Upload() {};
Upload::Upload(std::string status, std::string operation, std::string filename, std::string filetype) : 
	_status(status), 
	_operation(operation),
	_filename(filename),
	_filetype(filetype) {}


Upload::Upload(std::string status, std::string operation) : _status(status), _operation(operation) {}

//getters
const std::string Upload::getStatus() {return this->_status;}
const std::string Upload::getOperation() {return this->_operation;}
const std::string Upload::getFilename() {return this->_filename;}
const std::string Upload::getFiletype() {return this->_filetype;}

//setters
void Upload::setSuccess(std::string filename, std::string filetype) {
	this->_status = "success";
	this->_operation = "upload";
	this->_filename = filename;
	this->_filetype = filetype;
}

void Upload::setFailure() {
	this->_status = "failure";
	this->_operation = "upload";
}