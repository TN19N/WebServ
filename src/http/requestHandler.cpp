
# define BUFFER_SIZE 1024

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

static void __read_request_buffer_(Client* client, size_t size = BUFFER_SIZE)
{
    char buffer[size];
    ssize_t bytesReceived = recv(client->getFdOf(READ_END), buffer, size, 0);
	
    if (bytesReceived < 0)
		throw 500;
	else
		client->getBuffer().append(buffer, bytesReceived);
}
/*
static void __read_content_length_body_(Client* client, size_t size)
{
	char	buffer[size + 1];
	ssize_t	bytesReceived;
	
	if (size == 0)
	{
		client->getRequest()->state = REQUEST_READY;
		return;
	}
	switch (bytesReceived = recv(client->getFdOf(READ_END), buffer, size, 0)) {
		case -1 :
			throw 500;
		case 0 :
			throw "close connection";
		default :
			buffer[bytesReceived] = '\0';
			client->getRequest()->body = client->getRequest()->body + buffer;
			if (bytesReceived == size)
				client->getRequest()->readyToResponse = true;
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
	
	switch (bytesReceived = recv(client->getFdOf(READ_END), buffer, BUFFER_SIZE, 0))
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
		client->getRequest()->readyToResponse = true;
	}
}

static void __read_request_body_(Client* client)
{
	if (client->getRequest()->isChunked)
		__read_chunked_body_(client);
	else
		__read_content_length_body_(client, client->getRequest()->contentLength - client->getRequest()->body.length());
}
*/
void HTTP::requestHandler(Client* client, const Context* const configuration)
{
	const Context *location;
	
	__read_request_buffer_(client);
	if (client->getRequest() == nullptr && client->getBuffer().find(END_HEADERS) != std::string::npos)
	{
		client->newRequest(HTTP::requestParser(client));
		location = HTTP::blockMatchAlgorithm(client, configuration);
	}
	if (client->getRequest() != nullptr)
	{
#ifdef DEBUG
		std::cout << "===================== Request Parser =============================\n" ;
		std::cout << "Method: " << client->getRequest()->method << '\n' ;
		std::cout << "Path  : " << client->getRequest()->path << '\n' ;
		std::cout << "Query : " << client->getRequest()->query << '\n' ;
		std::cout << "Ext   : " << client->getRequest()->extension << '\n' ;
		std::cout << "          ------------------\n" ;
		for (std::map<std::string, std::string>::const_iterator b = client->getRequest()->headers.begin(),
				e = client->getRequest()->headers.end(); b != e; ++b)
			std::cout << b->first << ": " << b->second << '\n';
#endif
	}
/*
		if (client->getRequest()->state == READING_BODY)
			__read_request_body_(client);
		if (client->getRequest()->state == REQUEST_READY)
		{
			if (client->getRequest()->method == "GET")
				HTTP::getMethodHandler(location, client);
		}
	}
*/
}
