# include <string>
# include <map>
# include <iostream>

# include "keep/http.hpp"
# include "webserv/client.hpp"

static const std::string getConnectionState(const int& statusCode) {
    switch (statusCode) {
        case 404: return "close";
        default:  return "keep-alive";
    }
}

static void sendString(const Client* client, const std::string& str) {
    int bytesSent = 0;
    int totalBytesSent = 0;

    while (totalBytesSent < str.size()) {
        if ((bytesSent = send(client->getFd(), (str.c_str() + totalBytesSent), (str.size() - totalBytesSent), 0)) == -1) {
            throw 500;
        }
        totalBytesSent += bytesSent;
    }
}

bool HTTP::sendResponse(const Client* client, const int& statusCode, const std::string& body, const std::map<std::string, std::string>& headers) {
    std::string connectionState = getConnectionState(statusCode);

    sendString(client, "HTTP/1.1 " + std::to_string(statusCode) + " " + HTTP::getStatusCodeMessage(statusCode) + CRLF);
    sendString(client, std::string("Server: webserv/1.0.0") + CRLF);
    sendString(client, "Date: " + HTTP::getHttpDate() + CRLF);
    sendString(client, "Connection: " + connectionState + CRLF);
	sendString(client, "Content-Lenght: " + std::to_string(body.size()) + CRLF);
	sendString(client, std::string("Content-Type: text/html") + CRLF);

    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        sendString(client, it->first + ": " + it->second + CRLF);
    }

    sendString(client, CRLF + body);

    return (connectionState == "close");
}
