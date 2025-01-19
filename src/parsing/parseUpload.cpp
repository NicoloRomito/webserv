#include "../../include/includes.hpp"
#include "../../include/parseUpload.hpp"

std::string	parseFilename(std::string mpBody) {

	std::string fileName;
	size_t	npos, i = 0;

	npos = std::string::npos;

	fileName = mpBody.substr(mpBody.find("filename=\"") + 10,  npos);
	for (i = 0; fileName[i] != 0 && fileName[i] != '\"'; i++);

	if (fileName[i] == '\"')
		fileName = fileName.substr(0, i);
	else
		fileName = "";
	return fileName;
}

std::string	parseFileType(std::string mpBody) {

	std::string fileType;
	std::string toFind = "Content-Type: ";
	size_t	npos, i = 0;

	npos = std::string::npos;


	fileType = mpBody.substr(mpBody.find(toFind) + toFind.length(),  npos);
	for (i = 0; fileType[i] != 0 && fileType[i] != '\r'; i++);

	if (fileType[i] == '\r')
	{
		fileType = fileType.substr(0, i);
	}
	else
		fileType = "";
	return fileType;
}
