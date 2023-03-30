# include <string>
# include <sstream>

# include "webserv/response.hpp"
# include "webserv/http.hpp"


// * Constructor ****************************************************************************************************
// this is for cgi response
Response::Response() : download_file_fd(-1), statusCode(0) {}

Response::Response(bool keepAlive) : download_file_fd(-1), statusCode(0)
{
	this->keepAlive = keepAlive;
}
// this is for client response
Response::Response(const int statusCode, const bool keepAlive)
    : download_file_fd(-1), 
        statusCode(statusCode)
{
    std::stringstream statusCodeStr;
    statusCodeStr << statusCode;
    this->keepAlive = keepAlive;
    this->buffer.append("HTTP/1.1 " + statusCodeStr.str() + " " + HTTP::getStatusCodeMessage(statusCode) + CRLF);
    this->addHeader("Date", HTTP::getHttpDate());
    this->addHeader("Connection", (this->keepAlive) ? "keep-alive" : "close");
    this->addHeader("Server", "webserv/1.0.0");
}

// ******************************************************************************************************************

// * Methods ********************************************************************************************************
void Response::addHeader(const std::string& key, const std::string& value) {
    this->headers[key] = value;
    this->buffer += key + ": " + value + CRLF;
}

void Response::addBody(const std::string& body) {
    std::stringstream contentLengthStr;
    contentLengthStr << (this->contentLength = body.length());
    this->addHeader("Content-Length", contentLengthStr.str());
    this->buffer += CRLF + body;
}

void Response::initializeResponse() {
	if (statusCode == 0)
		throw "Please Initialize Status code initialize response" ;
    std::stringstream statusCodeStr;
    statusCodeStr << statusCode;
	this->buffer.append("HTTP/1.1 " + statusCodeStr.str() + " " + HTTP::getStatusCodeMessage(statusCode) + CRLF);
	this->addHeader("Date", HTTP::getHttpDate());
	this->addHeader("Connection", (keepAlive) ? "keep-alive" : "close");
	this->addHeader("Server", "webserv/1.0.0");
}
// ******************************************************************************************************************

// * Destructor *****************************************************************************************************
Response::~Response() {
	if (download_file_fd != -1) {
		close(download_file_fd);
    }
}
// ******************************************************************************************************************