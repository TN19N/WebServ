# define BUFFER_SIZE 480

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

static void __read_body_from_client_(Client* client, size_t size)
{
	char buffer[size + 1];
	int  bytesReceived;
	
	if (size == 0)
		return;
	switch (bytesReceived = recv(client->getFd(), buffer, size, 0)) {
		case -1 :
			throw 500;
		case 0 :
			throw "close connection";
		default :
			if (bytesReceived != size)
				throw 400;
			buffer[bytesReceived] = '\0';
			client->getRequest()->body = client->getRequest()->body + (buffer);
	}
}

static void __read_request_body_(Client* client)
{
	if (client->getRequest()->method != "POST")
		return;
	if (client->getRequest()->is_chunked)
		std::cout << "Is Chunked\n";
	else
		__read_body_from_client_(client, client->getRequest()->content_length - client->getRequest()->body.length());
}

void HTTP::requestHandler(Client* client, const Context* const configuration, std::vector<const Client*>& clientsToRemove) {
    try {
        readRequest(client);
        if (client->getRequest() == nullptr && client->getBuffer().find("\r\n\r\n") != std::string::npos)
		{
            client->newRequest(HTTP::request_parser(client->getBuffer()));
        }
        // add body
		__read_request_body_(client);
        // if body is done
         HTTP::blockMatchAlgorithm(client, client->getRequest(), configuration);
    } catch (int statusCode) {
        while (true) {
            try {
                if (HTTP::sendResponse(client, statusCode, getDefaultErrorPage(statusCode))) {
                    clientsToRemove.push_back(client);
                }
                break;
            } catch (const int& newStatusCode) {
                statusCode = newStatusCode;
            }
        }
    } catch (...) {
        clientsToRemove.push_back(client);
		std::cout << client->getRequest()->body << '\n';
    }
}
