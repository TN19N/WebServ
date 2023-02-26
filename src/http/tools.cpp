# include <string>
# include <time.h>

# include "webserv/http.hpp"

const std::string HTTP::getHttpDate(void) {
    time_t rawTime;
    struct tm* timeInfo;
    char buffer[80];

    time(&rawTime);
    timeInfo = gmtime(&rawTime);

    strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeInfo);
    return buffer;
}