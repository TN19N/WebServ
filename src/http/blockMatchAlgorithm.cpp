# include <string>
# include <vector>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <iostream>

# include "../../include/webserv/loadConfiguration.hpp"
# include "../../include/webserv/request.hpp"
# include "../../include/webserv/client.hpp"
# include "../../include/webserv/context.hpp"
# include "../../include/webserv/http.hpp"

static void isValidServer(const Context* context,  const Client* client, std::vector<const Context*>& servers) {
    if (context->getName() == SERVER_CONTEXT) {
        const std::string host = context->getDirective("host").front();
        const std::string port = context->getDirective("port").front();

        const struct sockaddr_storage& addr = client->getPeer();
	
		struct addrinfo *res, hints;
		
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		
        if (getaddrinfo(host.c_str(), port.c_str(), &hints, &res)  == -1) {
            std::cerr << "webserv: warning: getaddrinfo: " << strerror(errno) << std::endl;
            throw 500;
        }
		
        if (res->ai_family == addr.ss_family) {
            if (res->ai_addr->sa_family == AF_INET) {
                const struct sockaddr_in* addr4 = reinterpret_cast<const struct sockaddr_in*>(&addr);
                struct sockaddr_in* serverAddr4 = reinterpret_cast<struct sockaddr_in*>(res->ai_addr);
                if (addr4->sin_port == serverAddr4->sin_port) {
                    if (addr4->sin_addr.s_addr == serverAddr4->sin_addr.s_addr || serverAddr4->sin_addr.s_addr == INADDR_ANY) {
                        servers.push_back(context);
                    }
                }
            } else if (res->ai_family == AF_INET6) {
                const struct sockaddr_in6* addr6 = reinterpret_cast<const struct sockaddr_in6*>(&addr);
                struct sockaddr_in6* serverAddr6 = reinterpret_cast<struct sockaddr_in6*>(res->ai_addr);
                if (addr6->sin6_port == serverAddr6->sin6_port) {
                    if (memcmp(&(addr6->sin6_addr.__u6_addr), &(serverAddr6->sin6_addr), 16) == 0 || memcmp(&(serverAddr6->sin6_addr), &(in6addr_any), 16) == 0) {
						servers.push_back(context);
                    }
                }
            }
        }
    }

    for (std::vector<Context*>::const_iterator it = context->getChildren().begin(); it != context->getChildren().end(); ++it) {
        isValidServer(*it, client, servers);
    }
}

static int __compare_path_and_location_(const char *path, const char *location)
{
	while (*path && *path == *location)
	{ ++path; ++location; }
	if (*location == '\0' && *(location-1) == '/')
		return '/';
	return *path - *location;
}

static const Context* __get_match_server_context_(std::vector<const Context*> &servers, const std::string &host)
{
	std::map<const std::string, std::vector<std::string> >::const_iterator serv_name, not_found;
	std::vector<std::string>::const_iterator name, end;
	
	for (size_t i = 0; i < servers.size(); ++i)
	{
		serv_name = servers[i]->getDirectives().find(NAME_DIRECTIVE);
		not_found = servers[i]->getDirectives().end();
		if (serv_name != not_found)
		{
			for (name = serv_name->second.begin(), end = serv_name->second.end(); name != end; ++name)
				if (*name == host)
					return servers[i];
		}
	}
	return servers[0];
}

static const Context* __get_match_location_context_(const std::vector<Context*> &locations, const std::string &path)
{
	int		character;
	std::map<std::string, const Context*, std::greater<std::string> > locate;
	std::map<std::string, const Context*, std::greater<std::string> >::const_iterator begin, end;
	
	if (locations.empty())
		throw "no location found";
	for (size_t i = 0; i < locations.size(); ++i)
		locate.insert(std::make_pair(locations[i]->getArgs()[1], locations[i]));
	
	for (begin = locate.begin(), end = locate.end(); begin != end; ++begin)
	{
		if (begin->first == "/")
			return begin->second;
		character = __compare_path_and_location_(path.c_str(), begin->first.c_str());
		if (character == 0)
		{
			// Return 301 move to path/to'/' (slash at the end)
			throw "redirect to 301: /";
		}
		if (character == '/')
			return begin->second;
	}
	throw "no match any location: Forbidden";
}

static const Context* __get_location_context_(const Request *request, std::vector<const Context*> &servers)
{
	const Context *server = __get_match_server_context_(servers, request->headers.find("Host")->second);
	return __get_match_location_context_(server->getChildren(), request->path);
}

const Context* HTTP::blockMatchAlgorithm(const Client* client, const Context* const configuration) {
    std::vector<const Context*> servers;

    for (std::vector<Context*>::const_iterator it = configuration->getChildren().begin(); it != configuration->getChildren().end(); ++it) {
        isValidServer(*it, client, servers);
    }
    return __get_location_context_(client->getRequest(), servers);
}
