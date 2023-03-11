
# define BUFFER_SIZE 1024

# include <iostream>
# include <vector>
# include <map>

# ifdef DEBUG
# include <arpa/inet.h>
# endif

# include "../../include/webserv/client.hpp"
# include "../../include/webserv/context.hpp"
# include "../../include/webserv/http.hpp"

static void __read_request_buffer_from_client_(Client* client)
{
    char	buffer[BUFFER_SIZE];
    ssize_t	bytesReceived;

	bytesReceived = recv(client->getFdOf(READ_END), buffer, BUFFER_SIZE, 0);
    if (bytesReceived < 0)
		throw 500;
	else
		client->getBuffer().append(buffer, bytesReceived);
}

static void __check_allowed_method_(const std::vector<std::string> &allowed, std::string &method)
{
	std::vector<std::string>::const_iterator begin = allowed.begin();
	std::vector<std::string>::const_iterator end = allowed.end();
	
	for ( ; begin != end && *begin != method; ++begin) ;
	if (begin == end)
		throw 405;
}

void HTTP::requestHandler(Client* client, const Context* const configuration)
{
	Request			*request = client->getRequest();
	
	__read_request_buffer_from_client_(client);
	if (request == nullptr && client->getBuffer().find(END_HEADERS) != std::string::npos)
	{
		request = HTTP::requestParser(client);
		client->setRequest(request);
		request->location = HTTP::blockMatchAlgorithm(client, configuration);
		__check_allowed_method_(request->location->getDirectives().find(METHOD_DIRECTIVE)->second, request->method);
		request->fullPath.append(request->location->getDirective(ROOT_DIRECTIVE)[0]);
		request->fullPath.append(request->path);
	}
	if (request != nullptr)
	{
		if (request->state == READING_BODY)
			HTTP::readRequestBodyFromBuffer(client);
		if (request->state == REQUEST_READY)
		{
#ifdef DEBUG
		std::cout << "===================== Request Parser =============================\n" ;
		std::cout << "Method: " << request->method << '\n' ;
		std::cout << "Path  : " << request->path << '\n' ;
		std::cout << "fullPath  : " << request->fullPath << '\n' ;
		std::cout << "Ext   : " << request->extension << '\n' ;
		std::cout << "Query : " << request->query << '\n' ;
		std::cout << "          --------- headers ---------\n" ;
		for (std::map<std::string, std::string>::const_iterator b = request->headers.begin(),
				e = request->headers.end(); b != e; ++b)
			std::cout << b->first << ": " << b->second << '\n';
		std::cout << "          --------- Request Stat ---------\n" ;
		request->state == READING_BODY ? std::cout << "READING_BODY\n" : std::cout << "REQUEST_READY\n" ;
		std::cout << "          --------- Client BODY ---------\n" ;
		std::cout << "size: " << request->body.size() << '\n' ;
		std::cout << request->body << '\n' ;
		std::cout << "===================== Location =============================\n" ;
		std::cout << "Location: " << request->location->getArgs()[0] << '\n' ;
		for (std::map<std::string, std::vector<std::string> >::const_iterator
		b = request->location->getDirectives().begin(), e = request->location->getDirectives().end(); b != e; ++b)
		{
			std::cout << b->first << ": ";
			for (std::vector<std::string>::const_iterator bg = b->second.begin(), ed = b->second.end(); bg != ed; ++bg)
				std::cout << *bg << ' ' ;
			std::cout << '\n';
		}
		std::cout << "=============== END ===============\n" ;
#endif
			if (request->method == "GET")
				HTTP::getMethodHandler(client);
			else
				throw 1337;
		}
	}
}
