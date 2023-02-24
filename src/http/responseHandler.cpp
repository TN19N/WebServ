# include <string>
# include <map>
# include <iostream>

# include "webserv/http.hpp"
# include "webserv/client.hpp"

static const std::string getConnectionState(const int& statusCode) {
    switch (statusCode) {
        case 400: return "close";
        default:  return "keep-alive"; 
    }
}

static void sendString(const std::string& str) {
    int bytesSent = 0;
    int totalBytesSent = 0;

    while (totalBytesSent < str.size()) {
        if ((bytesSent = send(client->getFd(), (str.c_str() + totalBytesSent), (str.size() - totalBytesSent), 0)) == -1) {
            throw 500;
        }
        totalBytesSent += bytesSent;
    }
}

const bool HTTP::sendResponse(const Client* client, const int& statusCode, const std::string& body = "", const std::map<std::string, std::string>& headers = std::map<std::string, std::string>()) {
    std::string connectionState = getConnectionState(statusCode);

    sendString("HTTP/1.1 " + std::to_string(statusCode) + " " + HTTP::getStatusCodeMessage(statusCode) + CRLF);
    sendString("Server: webserv/1.0.0" + CRLF);
    sendString("Date: " + HTTP::getHttpDate() + CRLF);
    sendString("Connection: " + connectionState + CRLF);

    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        sendString(it->first + ": " + it->second + CRLF);
    }

    sendString(CRLF + body);

    return (connectionState == "close");
}

void HTTP::responseHandler(Client* client, Request* request, const Context* const configuration) {
    const Context* context = blockMatchAlgorithm(client, request, configuration);
}
