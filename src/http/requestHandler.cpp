# define BUFFER_SIZE 4096

# include <iostream>
# include <string>
# include <vector>
# include <iostream>
# include <map>

# ifdef DEBUG
# include <arpa/inet.h>
# endif

# include "../../include/webserv/client.hpp"
# include "../../include/webserv/context.hpp"
# include "../../include/webserv/request.hpp"
# include "../../include/webserv/http.hpp"

static const std::string getDefaultErrorPage(const int& statusCode) {
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

static void readRequest(Client* client) {
    char buffer[BUFFER_SIZE];
    int  bytesReceived = 0;
    switch (bytesReceived = recv(client->getFd(), buffer, BUFFER_SIZE, 0)) {
        case -1 :   
            throw 500;
        case 0 :
            throw "close connection";
        default :
            client->addBuffer(buffer, bytesReceived);
    }
}

void HTTP::requestHandler(Client* client, const Context* const configuration, std::vector<const Client*>& clientsToRemove) {
    try {
        readRequest(client);
        if (client->getRequest() == nullptr && client->getBuffer().find("\r\n\r\n") != std::string::npos) {
            client->newRequest(request_parser(client->getBuffer()));
        }
        // add body
        // if body is done
        // HTTP::blockMatchAlgorithm(client, client->getRequest(), )
    } catch (int statusCode) {
        while (true) {
            try {
                if (HTTP::sendResponse(client, statusCode, getDefaultErrorPage(statusCode)) == true) {
                    clientsToRemove.push_back(client);
                }
                break;
            } catch (const int& newStatusCode) {
                statusCode = newStatusCode;
            }
        }
    } catch (...) {
        clientsToRemove.push_back(client);
    }
}
