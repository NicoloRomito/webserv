#include "../../include/includes.hpp" // IWYU pragma: keep
#include "../../include/includeClasses.hpp" // IWYU pragma: keep

void	setMessageandRedirect(std::string& message, int statusCode, bool& isRedirect)
{
		switch (statusCode) {
		case 200:
			message = "OK";
			break;
		case 201:
			message = "Created";
			break;
		case 204:
			message = "No Content";
			break;
		case 301:
			message = "Moved Permanently";
			isRedirect = true;
			break;
		case 307:
			message = "Temporary redirect";
			isRedirect = true;
			break;
		case 400:
			message = "Bad Request";
			break;
		case 403:
			message = "Forbidden";
			break;
		case 404:
			message = "Not Found";
			break;
		case 405:
			message = "Method Not Allowed";
			break;
		case 413:
			message = "Content Too Large";
			break;
		case 422:
			message = "Unprocessable Content";
			break;
		case 500:
			message = "Internal Server Error";
			break;
		case 501:
			message = "Not Implemented";
			break;
		case 502:
			message = "Bad Gateway";
			break;
		default:
			break;
	}
}

const std::string setContentType(Request* req) {
	std::string contentType = "text/html";
	std::string extension = req->getUrlPath().substr(req->getUrlPath().find_last_of('.') + 1);
	if (extension == "html" || extension == "css")
		contentType = "text/" + extension;
	else if (extension == "js")
		contentType = "application/javascript";
	else if (extension == "jpg" || extension == "jpeg")
		contentType = "image/jpeg";
	else if (extension == "png")
		contentType = "image/png";
	else if (extension == "gif")
		contentType = "image/gif";
	else if (extension == "ico")
		contentType = "image/x-icon";
	else if (extension == "image/heic")
		contentType = "image/heic";
	else if (extension == "json")
		contentType = "application/json";
	else if (extension == "xml")
		contentType = "application/xml";
	else if (extension == "pdf")
		contentType = "application/pdf";
	else if (extension == "zip")
		contentType = "application/zip";
	else if (extension == "tar")
		contentType = "application/x-tar";
	else if (extension == "gz")
		contentType = "application/x-gzip";
	else if (extension == "txt")
		contentType = "text/plain";

	return contentType;
}