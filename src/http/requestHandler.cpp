
# include <iostream>
# include <vector>
# include <map>

# include "../../include/webserv/client.hpp"
# include "../../include/webserv/http.hpp"

# ifdef DEBUG
# include <arpa/inet.h>

static void __print_request_data_for_debug_(Request *request)
{
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
}
# endif

static void __check_allowed_method_(const Context* location, std::string &method)
{
	std::vector<std::string>::const_iterator begin;
	std::vector<std::string>::const_iterator end;
	const std::vector<std::string> *methods;
	
	methods = &(location->getDirectives().find(METHOD_DIRECTIVE)->second);
	for (begin = methods->begin(), end = methods->end() ; begin != end && *begin != method; ++begin) ;
	if (begin == end)
		throw 405;
}

void HTTP::requestHandler(Client* client, const Context* const configuration)
{
	Request			*request;
	
	HTTP::readRequestBufferFromClient(client);
	HTTP::readBodyFromBuffer(client);
	if (client->getRequest() == 0 && client->getBuffer().find(END_HEADERS) != std::string::npos)
		client->setState(READING_BODY);
	switch (client->getState())
	{
		case UPLOADING_FILE:
			if (write(request->upload_file_fd, request->body.c_str(), request->body.size()) < 0)
			{
				close(request->upload_file_fd);
				throw 500;
			}
			request->body.clear();
			if (request->state == BODY_READY)
			{
				client->setState(READING_REQUEST);
				close(request->upload_file_fd);
				request->upload_file_fd = 0;
				client->setResponse(new Response(200, false));
				client->switchState();
			}
			break;
		case READING_BODY:
			client->setState(READING_REQUEST);
			request = HTTP::requestParser(client);
			client->setRequest(request);
			HTTP::readBodyFromBuffer(client);
			request->location = HTTP::blockMatchAlgorithm(client, configuration);
			__check_allowed_method_(request->location, request->method);
			request->fullPath.append(request->location->getDirective(ROOT_DIRECTIVE)[0]);
			request->fullPath.append(request->path);
			if (request->method == "GET")
				HTTP::getMethodHandler(client);
			else if (request->method == "POST")
				HTTP::postMethodHandler(client);
			else // else mean delete method
				HTTP::deleteMethodHandler(client);
	}
}
