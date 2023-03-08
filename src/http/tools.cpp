# include <string>
# include <vector>
# include <poll.h>
# include <time.h>
# include <iostream>

# include "webserv/http.hpp"
# include "webserv/client.hpp"

// * Functions *************************************************************************************************************************
Client* HTTP::getClientWithFd(const int fd, const std::vector<Client*>& clients) {
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i]->isCgi()) {
            if (clients[i]->getState() == SENDING_REQUEST) {
                if (clients[i]->getFdOf(WRITE_END) == fd) {
                    return clients[i];
                }
            } else if (clients[i]->getState() == RIDING_RESPONSE) {
                if (clients[i]->getFdOf(READ_END) == fd) {
                    return clients[i];
                }
            }
        } else {
            if (clients[i]->getSocketFd() == fd) {
                return clients[i];
            }
        }
    }
    return nullptr; // never be reached (i hope so :D)
}

const std::string HTTP::getDefaultErrorPage(const int statusCode) {
    return (
        "<!DOCTYPE html>\n"
        "<html>\n"
        "   <head>\n"
        "       <title>" + std::to_string(statusCode) + " " + HTTP::getStatusCodeMessage(statusCode) + "</title>\n"
        "   </head>\n"
        "   <body>\n"
        "       <h1>" + std::to_string(statusCode) + " " + HTTP::getStatusCodeMessage(statusCode) + "</h1>\n"
        "       <hr>\n"
        "       webserv/1.0.0\n"
        "   </body>\n"
        "</html>\n"
    );
}

const std::string HTTP::getHttpDate() {
    time_t rawTime;
    struct tm* timeInfo;
    char buffer[80];

    if (time(&rawTime) == -1) {
        std::cerr << "webserv: time(): " << strerror(errno) << std::endl;
        throw 500;
    }

    timeInfo = gmtime(&rawTime);
    if (timeInfo == nullptr) {
        std::cerr << "webserv: gmtime(): " << strerror(errno) << std::endl;
        throw 500;
    }

    size_t res = strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeInfo);
    buffer[res] = '\0';
    return buffer;
}

const std::string HTTP::getStatusCodeMessage(const int statusCode) {
    switch (statusCode) {
        // 1xx: Informational
        case 100:   return "Continue";
        case 101:   return "Switching Protocols";
        case 102:   return "Processing";
        case 103:   return "Early Hints";
        // 2xx: Success
        case 200:   return "OK";
        case 201:   return "Created";
        case 202:   return "Accepted";
        case 203:   return "Non-Authoritative Information";
        case 204:   return "No Content";
        case 205:   return "Reset Content";
        case 206:   return "Partial Content";
        case 207:   return "Multi-Status";
        case 208:   return "Already Reported";
        case 226:   return "IM Used";
        // 3xx: Redirection
        case 300:   return "Multiple Choices";
        case 301:   return "Moved Permanently";
        case 302:   return "Found";
        case 303:   return "See Other";
        case 304:   return "Not Modified";
        case 305:   return "Use Proxy";
        case 306:   return "Switch Proxy";
        case 307:   return "Temporary Redirect";
        case 308:   return "Permanent Redirect";
        // 4xx: Client Error
        case 400:   return "Bad Request";
        case 401:   return "Unauthorized";
        case 402:   return "Payment Required";
        case 403:   return "Forbidden";
        case 404:   return "Not Found";
        case 405:   return "Method Not Allowed";
        case 406:   return "Not Acceptable";
        case 407:   return "Proxy Authentication Required";
        case 408:   return "Request Timeout";
        case 409:   return "Conflict";
        case 410:   return "Gone";
        case 411:   return "Length Required";
        case 412:   return "Precondition Failed";
        case 413:   return "Payload Too Large";
        case 414:   return "URI Too Long";
        case 415:   return "Unsupported Media Type";
        case 416:   return "Range Not Satisfiable";
        case 417:   return "Expectation Failed";
        case 418:   return "I'm a teapot";
        case 421:   return "Misdirected Request";
        case 422:   return "Unprocessable Entity";
        case 423:   return "Locked";
        case 424:   return "Failed Dependency";
        case 425:   return "Too Early";
        case 426:   return "Upgrade Required";
        case 428:   return "Precondition Required";
        case 429:   return "Too Many Requests";
        case 431:   return "Request Header Fields Too Large";
        case 451:   return "Unavailable For Legal Reasons";
        // 5xx: Server Error
        case 500:   return "Internal Server Error";
        case 501:   return "Not Implemented";
        case 502:   return "Bad Gateway";
        case 503:   return "Service Unavailable";
        case 504:   return "Gateway Timeout";
        case 505:   return "HTTP Version Not Supported";
        case 506:   return "Variant Also Negotiates";
        case 507:   return "Insufficient Storage";
        case 508:   return "Loop Detected";
        case 510:   return "Not Extended";
        case 511:   return "Network Authentication Required";
        // Unknown
        default:    return "Unknown";
    }
}
// *************************************************************************************************************************************