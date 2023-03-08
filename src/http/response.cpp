# include <string>

# include "../../include/webserv/response.hpp"
# include "../../include/webserv/http.hpp"

# define CRLF "\r\n"

// * Constructor ****************************************************************************************************
Response::Response(const int statusCode, const bool keepAlive) 
    : statusCode(statusCode),
        keepAlive(keepAlive),
        contentLength(0),
        protocol("HTTP/1.1"),
        buffer("HTTP/1.1 " + std::to_string(statusCode) + " " + HTTP::getStatusCodeMessage(statusCode) + CRLF)
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
    // Nothing to do
}
// ******************************************************************************************************************