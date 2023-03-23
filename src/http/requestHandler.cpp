
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
	for (IBase::Headers::const_iterator b = request->headers.begin(),
			e = request->headers.end(); b != e; ++b)
		std::cout << b->first << ": " << b->second << '\n';
	std::cout << "===================== Location =============================\n" ;
	std::cout << "Location: " << request->location->getArgs()[0] << '\n' ;
	for (Context::Directives::const_iterator b = request->location->getDirectives().begin(),
			e = request->location->getDirectives().end(); b != e; ++b)
	{
		if (b->first[0] == '.')
			continue;
		std::cout << b->first << ": ";
		for (std::vector<std::string>::const_iterator bg = b->second.begin(), ed = b->second.end(); bg != ed; ++bg)
			std::cout << *bg << ' ' ;
		std::cout << '\n';
	}
	std::cout << "=============== END ===============\n" ;
}

// static void __print_response_data_for_debug_(Response *response) {
// 	std::cout << "===================== Response =============================\n" ;
// 	std::cout << "Status: " << response->statusCode << std::endl;
// 	std::cout << "download_file_fd: " << response->download_file_fd << std::endl;
// 	std::cout << "keepAlive: " << response->keepAlive << std::endl;
// 	std::cout << "contentLength: " << response->contentLength << std::endl;
// 	std::cout << "	--------- headers ---------	 " << std::endl;
// 	for (IBase::Headers::const_iterator b = response->headers.begin(), e = response->headers.end(); b != e; ++b) {
// 		std::cout << '\t' << b->first << ": " << b->second << std::endl;
// 	}
// }
# endif

static unsigned long long __calc_max_body_size_(const Context::Directives::const_iterator& maxSize)
{
	const char	*str = maxSize->second[0].c_str();
	size_t		max = 0;
	
	while (*str != 'M')
	{
		max += max * 10 ;
		max += *str - '0' ;
		++str;
	}
	return max * 1024 * 1024 ;
}

static bool __read_buffer_from_client_(Client* client)
{
	char	buffer[BUFFER_SIZE];
	ssize_t	readSize;
	
	readSize = read(client->getFdOf(READ_END), buffer, BUFFER_SIZE);
	if (readSize < 0) {
		throw 500;
	} else if (readSize == 0 && client->isCgi()) {
		return true;
	}
	client->getBuffer().append(buffer, readSize);
	return false;
}

static void __check_allowed_method_(const Context* location, std::string &method)
{
	std::vector<std::string>::const_iterator begin, end;
	const std::vector<std::string> 			*methods;
	
	methods = &(location->getDirectives().find(METHOD_DIRECTIVE)->second);
	for (begin = methods->begin(), end = methods->end() ; begin != end && *begin != method; ++begin) ;
	if (begin == end)
		throw 405;
}

static Client* __client_request_handler_(Client* client, const Context* const configuration)
{
	Client	*cgi = 0;
	Request	*request;

	if (client->getRequest() == 0 && client->getBuffer().find(END_HEADERS) != std::string::npos) {
		request = reinterpret_cast<Request*>(HTTP::baseParser(client));
		client->setRequest(request);
		try {
			client->getRequest()->keepAlive = (client->getRequest()->headers.at("Connection") != "close");
		} catch (std::out_of_range&) {}

		request->location = HTTP::blockMatchAlgorithm(client, configuration);
		
		__check_allowed_method_(request->location, request->method);
		request->fullPath.append(request->location->getDirective(ROOT_DIRECTIVE).at(0));
		request->fullPath.append(request->path);
		request->maxBodySize = __calc_max_body_size_(request->location->getDirectives().find(SIZE_DIRECTIVE));

# ifdef DEBUG
		__print_request_data_for_debug_(request);
# endif

		if (request->method == "GET") {
			return HTTP::getMethodHandler(client);
		} else if (request->method == "POST") {
			cgi = HTTP::postMethodHandler(client);
		} else if (request->method == "DELETE") {
			return HTTP::deleteMethodHandler(client);
		}
	}

	request = client->getRequest();
	if (request) {
		if (request->state == CREATING) {
			HTTP::readBodyFromBuffer(client);
		}
		
		if (request->upload_file_fd != -1) {
			if (write(request->upload_file_fd, request->body.c_str(), request->body.size()) < 0) {
				unlink(request->upload_file_name.c_str());
				close(request->upload_file_fd);
				request->upload_file_fd = -1;
				throw 500;
			}
			request->body.clear();
			
			if (request->state == READY) {
				client->setResponse(new Response(201, request->keepAlive));
				client->getResponse()->addBody("Created with success");
				client->switchState();
			}
		}
	}
	return cgi;
}

static Client* __cgi_response_handler_(Client* cgi, bool cgiFinish) {
	Response	*response = cgi->getResponse();

	if (response == 0 && cgi->getBuffer().find(END_HEADERS) != std::string::npos) {
		response = reinterpret_cast<Response*>(HTTP::baseParser(cgi));
		cgi->setResponse(response);
	}
	if (response) {
		HTTP::readBodyFromBuffer(cgi);
		if (cgiFinish) {
			HTTP::convertCgiResponseToClientResponse(cgi);
			return cgi;
		}
	} else if (cgiFinish) {
		throw 502;
	}
	return nullptr;
}

Client* HTTP::requestHandler(Client* client, const Context* const configuration) {
	bool	cgiFinish = false;

	if (__read_buffer_from_client_(client))
		cgiFinish = true;
	if (client->isCgi())
		return __cgi_response_handler_(client, cgiFinish);
	else
		return __client_request_handler_(client, configuration);
}
