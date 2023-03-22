# include <string>
# include <vector>
# include <time.h>
# include <iostream>

# include "../../include/webserv/http.hpp"

// * Functions *************************************************************************************************************************

static int __get_status_code_from_cgi_status_(const char *status)
{
	int statusCode = 0;
	
	while (*status != ' ') {
		if (*status < '0' || *status > '9')
			throw 400;
		statusCode += (statusCode * 10) + *status - '0';
		++status;
	}
	return statusCode;
}

static bool __is_forbidden_to_send_this_header_to_client_(const char *key)
{
	const char *headers[5] = {"Connection", "Content-Length", "Date", "Server", nullptr};
	
	for (int i = 0; headers[i]; ++i) {
		if (HTTP::strcmp(key, headers[i]) == 0) {
			return true;
		}
	}
	return false;
}

void HTTP::convertCgiResponseToClientResponse(Client *cgi)
{
	IBase::Headers::iterator	header, notFound;
	Response					*response;
	Client 						*client = cgi->getCgiToClient();
	int							statusCode;
	
	if (cgi->getResponse() == nullptr) {
		throw 500;
	}
	
	notFound = cgi->getResponse()->headers.end();
	header = cgi->getResponse()->headers.find("Status");
	if (header == notFound) {
		header = cgi->getResponse()->headers.find("Location");
		if (header != notFound) {
			statusCode = 301;
		} else {
			statusCode = 200;
		}
	} else {
		statusCode = __get_status_code_from_cgi_status_(header->second.c_str());
	}

	response = new Response(statusCode, client->getRequest()->keepAlive);
	client->setResponse(response);
	
	for (header = cgi->getResponse()->headers.begin(); header != notFound; ++header)
	{
		if (__is_forbidden_to_send_this_header_to_client_(header->first.c_str()))
			continue;
		response->addHeader(header->first, header->second);
	}
	response->addBody(cgi->getResponse()->body);
	client->switchState();
}

int HTTP::strcmp(const char *s1, const char *s2) {
	while (*s1 && *s1 == *s2)
			{ ++s1; ++s2; }
	return *s1 - *s2;
}

const char * HTTP::getExtensionFromPath(const char *path) {
	while (*path)
		++path;
	while (*path != '/' && *path != '.')
		--path;
	if (*path == '.' && *(path-1) != '/')
		return path;
	return "";
}

int HTTP::parseContentLength(const char *str)
{
	const char 	*checker = str;
	long int	res = 0;
	
	if (*checker == '\0')
		throw 400;
	while (*checker)
	{
		if (*checker < '0' || *checker > '9')
			throw 400;
		++checker;
	}
	while (*str && res < INT_MAX)
	{
		res = res * 10 + *str - '0' ;
		++str;
	}
	if (*str == '\0')
		return res;
	throw 400;
}

Client* HTTP::getClientWithFd(const int fd, const std::vector<Client*>& clients) {
    for (size_t i = 0; i < clients.size(); ++i) {
		if (clients[i]->getState() == SENDING_REQUEST || clients[i]->getState() == SENDING_RESPONSE) {
			if (clients[i]->getFdOf(WRITE_END) == fd) {
				return clients[i];
			}
		} else if (clients[i]->getState() == READING_RESPONSE || clients[i]->getState() == READING_REQUEST) {
			if (clients[i]->getFdOf(READ_END) == fd) {
				return clients[i];
			}
		}
    }
    return nullptr; // never be reached (I hope so :D)
}

const std::string HTTP::getDefaultErrorPage(const int statusCode) {
    return (
        "<style>\n"
        "   @import url('https://fonts.googleapis.com/css2?family=Press+Start+2P&display=swap');\n"
        "   * {\n"
        "       padding: 0;\n"
        "       margin: 0;\n"
        "       box-sizing: border-box;\n"
        "       font-family: 'Press Start 2P';\n"
        "       color: #FFFFFF;\n"
        "       text-align: center;\n"
        "   }\n"
        "   body {\n"
        "       background-color: #000000;\n"
        "       background-image: url(\"data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='42' height='58' viewBox='0 0 42 58'%3E%3Cg fill='%23dddcdd' fill-opacity='0.23'%3E%3Cpath fill-rule='evenodd' d='M12 18h12v18h6v4H18V22h-6v-4zm-6-2v-4H0V0h36v6h6v36h-6v4h6v12H6v-6H0V16h6zM34 2H2v8h24v24h8V2zM6 8a2 2 0 1 0 0-4 2 2 0 0 0 0 4zm8 0a2 2 0 1 0 0-4 2 2 0 0 0 0 4zm8 0a2 2 0 1 0 0-4 2 2 0 0 0 0 4zm8 0a2 2 0 1 0 0-4 2 2 0 0 0 0 4zm0 8a2 2 0 1 0 0-4 2 2 0 0 0 0 4zm0 8a2 2 0 1 0 0-4 2 2 0 0 0 0 4zm0 8a2 2 0 1 0 0-4 2 2 0 0 0 0 4zM2 50h32v-8H10V18H2v32zm28-6a2 2 0 1 0 0 4 2 2 0 0 0 0-4zm-8 0a2 2 0 1 0 0 4 2 2 0 0 0 0-4zm-8 0a2 2 0 1 0 0 4 2 2 0 0 0 0-4zm-8 0a2 2 0 1 0 0 4 2 2 0 0 0 0-4zm0-8a2 2 0 1 0 0 4 2 2 0 0 0 0-4zm0-8a2 2 0 1 0 0 4 2 2 0 0 0 0-4zm0-8a2 2 0 1 0 0 4 2 2 0 0 0 0-4z'/%3E%3C/g%3E%3C/svg%3E\");\n"
        "   }\n"
        "   section.notFound {\n"
        "       display: flex;\n"
        "       justify-content: center;\n"
        "       align-items: center;\n"
        "       margin: 0 5%;\n"
        "       height: 100vh;\n"
        "   }\n"
        "   section.notFound h1 {\n"
        "       color: red;\n"
        "       font-size: 100px;\n"
        "   }\n"
        "   section.notFound h2 {\n"
        "       font-size: 50px;\n"
        "   }\n"
        "   section.notFound h1, h2, h3 {\n"
        "       margin-bottom: 40px;\n"
        "   }\n"
        "   div.text {\n"
        "       height: 50vh;\n"
        "   }\n"
        "   div.text a {\n"
        "       text-decoration: none;\n"
        "       margin-right: 20px;\n"
        "   }\n"
        "   div.text a:hover {\n"
        "       color: red;\n"
        "       text-decoration: underline;\n"
        "   }\n"
        "   @media only screen and (max-width: 768px) {\n"
        "       section.notFound {\n"
        "           flex-direction: column;\n"
        "           justify-content: space-around;\n"
        "       }\n"
        "       section.notFound div.img img {\n"
        "           width: 70vw;\n"
        "           height: auto;\n"
        "       }\n"
        "       section.notFound h1 {\n"
        "           font-size: 50px;\n"
        "       }\n"
        "       section.notFound h2 {\n"
        "           font-size: 25px;\n"
        "       }\n"
        "       div.text a:active {\n"
        "       color: red;\n"
        "       text-decoration: underline;\n" 
        "       }\n"
        "   }\n"
        "</style>\n"
        "<body>\n"
        "   <section class=\"notFound\">\n"
        "       <div class=\"img\">\n"
        "       <img src=\"https://assets.codepen.io/5647096/backToTheHomepage.png\" alt=\"Back to the Homepage\"/>\n"
        "       <img src=\"https://assets.codepen.io/5647096/Delorean.png\" alt=\"El Delorean, El Doc y Marti McFly\"/>\n"
        "       </div>\n"
        "       <div class=\"text\">\n"
        "       <h1>" + std::to_string(statusCode) + "</h1>\n"
        "       <h2>" + HTTP::getStatusCodeMessage(statusCode) + "</h2>\n"
        "       <h3>BACK TO HOME?</h3>\n"
        "       <a href=\"/\" class=\"yes\">YES</a>\n"
        "       <a href=\"https://www.youtube.com/watch?v=NUYvbT6vTPs\">NO</a>\n"
        "       </div>\n"
        "   </section>\n"
        "</body>\n"
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

const std::string HTTP::urlDecoding(const std::string& url) {
    std::string result;

    for (size_t i = 0; i < url.length(); i++) {
        if (url[i] == '%' && i + 2 < url.length()) {
            std::string hex = url.substr(i + 1, 2);
            char c = static_cast<char>(strtol(hex.c_str(), nullptr, 16));
            result += c;
            i += 2;
        } else {
            result += url[i];
        }
    }
    return result;
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