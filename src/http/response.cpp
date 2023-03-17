# include <string>

# include "../../include/webserv/response.hpp"
# include "../../include/webserv/http.hpp"


// * Constructor ****************************************************************************************************
Response::Response(const int statusCode, const bool keepAlive) 
    : download_file_fd(-1), 
        statusCode(statusCode), 
        keepAlive(keepAlive), 
        contentLength(0),
        buffer("HTTP/1.1 " + std::to_string(statusCode) + " " + HTTP::getStatusCodeMessage(statusCode) + CRLF),
        state(CREATING)
{
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