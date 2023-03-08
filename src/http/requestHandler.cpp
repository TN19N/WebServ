# define BUFFER_SIZE 40

# include <iostream>
# include <string>
# include <vector>
# include <iostream>
# include <map>

# ifdef DEBUG
# include <arpa/inet.h>
# endif

# include "webserv/client.hpp"
# include "webserv/context.hpp"
# include "keep/request.hpp"
# include "keep/http.hpp"

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

static void __read_content_length_body_(Client* client, size_t size)
{
	char	buffer[size + 1];
	ssize_t	bytesReceived;
	
	if (size == 0)
	{
		client->getRequest()->ready_to_response = true;
		return;
	}
	switch (bytesReceived = recv(client->getFd(), buffer, size, 0)) {
		case -1 :
			throw 500;
		case 0 :
			throw "close connection";
		default :
			buffer[bytesReceived] = '\0';
			client->getRequest()->body = client->getRequest()->body + buffer;
			if (bytesReceived == size)
				client->getRequest()->ready_to_response = true;
	}
}

static void __parse_chunked_body_(const char *chunked, std::string& body)
{


}

static void __read_chunked_body_(Client* client)
{
	std::string	body;
	char		buffer[BUFFER_SIZE + 1];
	ssize_t		bytesReceived;
	
	switch (bytesReceived = recv(client->getFd(), buffer, BUFFER_SIZE, 0))
	{
		case -1 :
			throw 500;
		case 0 :
			throw "close connection";
		default :
			buffer[bytesReceived] = '\0';
			client->getRequest()->body.append(buffer);
	}
	if (client->getRequest()->body.find(END_CHUNKED) != std::string::npos)
	{
		__parse_chunked_body_(client->getRequest()->body.c_str(), body);
		client->getRequest()->body = body;
		client->getRequest()->ready_to_response = true;
	}
}

static void __read_request_body_(Client* client)
{
	if (client->getRequest()->is_chunked)
		__read_chunked_body_(client);
	else
		__read_content_length_body_(client, client->getRequest()->content_length - client->getRequest()->body.length());
}

void HTTP::requestHandler(Client* client, const Context* const configuration, std::vector<const Client*>& clientsToRemove) {
    try {
		if (client->getRequest() == nullptr || client->getRequest()->missing_headers)
        	readRequest(client);
        if (client->getRequest() == nullptr && client->getBuffer().find(END_HEADERS) != std::string::npos)
		{
            client->newRequest(HTTP::request_parser(client));
			client->getRequest()->missing_headers = false;
        }
		if (client->getRequest() != nullptr)
		{
			if ( ! client->getRequest()->ready_to_response)
				__read_request_body_(client);
			if (client->getRequest()->ready_to_response)
				HTTP::request_handler(client, configuration);
		}
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
    }
}
