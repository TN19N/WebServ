# include <string>

# include "../../include/webserv/response.hpp"
# include "../../include/webserv/http.hpp"


// * Constructor ****************************************************************************************************
// this is for cgi response
Response::Response() {}

// this is for client response
Response::Response(const int statusCode, const bool keepAlive)
    : download_file_fd(-1), 
        statusCode(statusCode)
{
    this->keepAlive = keepAlive;
    this->buffer.append("HTTP/1.1 " + std::to_string(statusCode) + " " + HTTP::getStatusCodeMessage(statusCode) + CRLF);
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
    this->contentLength = body.length();
    this->addHeader("Content-Length", std::to_string(this->contentLength));
    this->buffer += CRLF + body;
}
// ******************************************************************************************************************

// * Destructor *****************************************************************************************************
Response::~Response() {
	if (download_file_fd != -1) {
		close(download_file_fd);
    }
}
// ******************************************************************************************************************