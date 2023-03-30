# include <string>
# include <vector>
# include <time.h>
# include <iostream>
# include <sstream>
# include <string.h>

# include "webserv/http.hpp"

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
	const char *headers[6] = {"Connection", "Content-Length", "Date", "Server", "Status", NULL};

	for (int i = 0; headers[i]; ++i) {
		if (HTTP::strcmp(key, headers[i]) == 0) {
			return true;
		}
	}
	return false;
}

void HTTP::convertCgiResponseToClientResponse(Client *cgi)
{
	IBase::Headers::iterator			header, notFound;
	std::vector<std::string>::iterator	setCookie, endCookie;
	Response							*clientRes;
	Response							*cgiRes = cgi->getResponse();
	Client								*client = cgi->getCgiToClient();
	int									statusCode;

	if (cgiRes == NULL) {
		throw 500;
	}

	notFound = cgiRes->headers.end();
	header = cgiRes->headers.find("Status");
	if (header == notFound) {
		header = cgiRes->headers.find("Location");
		if (header != notFound) {
			statusCode = 301;
		} else {
			statusCode = 200;
		}
	} else {
		statusCode = __get_status_code_from_cgi_status_(header->second.c_str());
	}
	clientRes = new Response(statusCode, client->getRequest()->keepAlive);
	client->setResponse(clientRes);
	for (header = cgiRes->headers.begin(); header != notFound; ++header)
	{
		if (__is_forbidden_to_send_this_header_to_client_(header->first.c_str()))
			continue;
		clientRes->addHeader(header->first, header->second);
	}
	for (setCookie = cgiRes->setCookie.begin(), endCookie = cgiRes->setCookie.end(); setCookie != endCookie; ++setCookie) {
		clientRes->addHeader("Set-Cookie", *setCookie);
	}
	clientRes->addBody(cgiRes->body);
}

size_t HTTP::getCurrentTimeOnMilliSecond()
{
	timeval	tv;
	
	gettimeofday(&tv, 0);
	
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
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
		if (clients[i]->getState() == TO_CGI) {
			if (clients[i]->getFdOf(WRITE_END) == fd) {
				return clients[i];
			}
			if (clients[i]->getFdOf(READ_END) == fd) {
				return clients[i];
			}
		} else if (clients[i]->getState() == TO_CLIENT) {
			if (clients[i]->getFdOf(WRITE_END) == fd) {
				return clients[i];
			}
		} else if (clients[i]->getState() == FROM_CGI || clients[i]->getState() == FROM_CLIENT) {
			if (clients[i]->getFdOf(READ_END) == fd) {
				return clients[i];
			}
		}
    }
    return NULL; // never be reached (I hope so :D)
}

const std::string HTTP::getDefaultErrorPage(const int statusCode) {
	std::stringstream statusCodeStr;
	statusCodeStr << statusCode;
    return ("<style> @import 'https://fonts.googleapis.com/css?family=Inconsolata';\n"
			"\n"
			"html { min-height: 100%; }"
			"body {\n"
			"  box-sizing: border-box;\n"
			"  height: 100%;\n"
			"  background-color: #000000;\n"
			"  background-image: radial-gradient(#11581E, #041607), url(\"https://media.giphy.com/media/oEI9uBYSzLpBK/giphy.gif\");\n"
			"  background-repeat: no-repeat;\n"
			"  background-size: cover;\n"
			"  font-family: 'Inconsolata', Helvetica, sans-serif;\n"
			"  font-size: 1.5rem;\n"
			"  color: rgba(128, 255, 128, 0.8);\n"
			"  text-shadow:\n"
			"      0 0 1ex rgba(51, 255, 51, 1),\n"
			"      0 0 2px rgba(255, 255, 255, 0.8);\n"
			"}\n"
			"\n"
			".noise {\n"
			"  pointer-events: none;\n"
			"  position: absolute;\n"
			"  width: 100%;\n"
			"  height: 100%;\n"
			"  background-image: url(\"https://media.giphy.com/media/oEI9uBYSzLpBK/giphy.gif\");\n"
			"  background-repeat: no-repeat;\n"
			"  background-size: cover;\n"
			"  z-index: -1;\n"
			"  opacity: .02;\n"
			"}\n"
			"\n"
			".overlay {\n"
			"  pointer-events: none;\n"
			"  position: absolute;\n"
			"  width: 100%;\n"
			"  height: 100%;\n"
			"  background:\n"
			"      repeating-linear-gradient(\n"
			"      180deg,\n"
			"      rgba(0, 0, 0, 0) 0,\n"
			"      rgba(0, 0, 0, 0.3) 50%,\n"
			"      rgba(0, 0, 0, 0) 100%);\n"
			"  background-size: auto 4px;\n"
			"  z-index: 1;\n"
			"}\n"
			"\n"
			".overlay::before {\n"
			"  content: \"\";\n"
			"  pointer-events: none;\n"
			"  position: absolute;\n"
			"  display: block;\n"
			"  top: 0;\n"
			"  left: 0;\n"
			"  right: 0;\n"
			"  bottom: 0;\n"
			"  width: 100%;\n"
			"  height: 100%;\n"
			"  background-image: linear-gradient(\n"
			"      0deg,\n"
			"      transparent 0%,\n"
			"      rgba(32, 128, 32, 0.2) 2%,\n"
			"      rgba(32, 128, 32, 0.8) 3%,\n"
			"      rgba(32, 128, 32, 0.2) 3%,\n"
			"      transparent 100%);\n"
			"  background-repeat: no-repeat;\n"
			"  animation: scan 7.5s linear 0s infinite;\n"
			"}\n"
			"\n"
			"@keyframes scan {\n"
			"  0%        { background-position: 0 -100vh; }\n"
			"  35%, 100% { background-position: 0 100vh; }\n"
			"}\n"
			"\n"
			".terminal {\n"
			"  box-sizing: inherit;\n"
			"  position: absolute;\n"
			"  height: 100%;\n"
			"  width: 1000px;\n"
			"  max-width: 100%;\n"
			"  padding: 4rem;\n"
			"  text-transform: uppercase;\n"
			"}\n"
			"\n"
			".output {\n"
			"  color: rgba(128, 255, 128, 0.8);\n"
			"  text-shadow:\n"
			"      0 0 1px rgba(51, 255, 51, 0.4),\n"
			"      0 0 2px rgba(255, 255, 255, 0.8);\n"
			"}\n"
			"\n"
			".output::before {\n"
			"  content: \"> \";\n"
			"}\n"
			"\n"
			"a {\n"
			"  color: #fff;\n"
			"  text-decoration: none;\n"
			"}\n"
			"\n"
			"a::before {\n"
			"  content: \"[\";\n"
			"}\n"
			"\n"
			"a::after {\n"
			"  content: \"]\";\n"
			"}\n"
			"\n"
			".errorcode {\n"
			"  color: white;\n"
			"}\n"
			"</style>\n"
			"<div class=\"noise\"></div>\n"
			"<div class=\"overlay\"></div>\n"
			"<div class=\"terminal\">\n"
			"  <h1><span class=\"errorcode\">" + statusCodeStr.str() +  "</span> " + HTTP::getStatusCodeMessage(statusCode) + "  </h1>\n"
			"  <p class=\"output\">Good luck.</p>\n"
			"</div>");
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
    if (timeInfo == NULL) {
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
            char c = static_cast<char>(strtol(hex.c_str(), NULL, 16));
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