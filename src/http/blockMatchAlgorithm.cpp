# include <string>
# include <vector>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <iostream>

# include "../../include/webserv/configuration.hpp"
# include "../../include/webserv/request.hpp"
# include "../../include/webserv/client.hpp"
# include "../../include/webserv/context.hpp"
# include "../../include/webserv/http.hpp"

static void isValidServer(const Context* context,  const Client* client, std::vector<const Context*>& servers) {
	const std::string host = context->getDirective("host").at(0);
	const std::string port = context->getDirective("port").at(0);

	const struct sockaddr_storage& addr = client->getPeerAddr();

	struct addrinfo *res, hints;
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(host.c_str(), port.c_str(), &hints, &res)  == -1) {
		std::cerr << "webserv: warning: getaddrinfo: " << strerror(errno) << std::endl;
		throw 500;
	}

	if (res->ai_family == AF_INET6) {
		struct addrinfo* tmp = res;
		while (tmp->ai_next != NULL && tmp->ai_next->ai_family != AF_INET) {
			tmp = tmp->ai_next;
		}
		if (tmp->ai_next != NULL) {
			struct addrinfo* tmp2 = tmp->ai_next;
			tmp->ai_next = tmp->ai_next->ai_next;
			tmp2->ai_next = res;
			res = tmp2;
		}
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
	freeaddrinfo(res);
}

static const Context* __get_match_server_context_(std::vector<const Context*> &servers, const std::string &host)
{
	std::map<std::string, std::vector<std::string> >::const_iterator serv_name, not_found;
	std::vector<std::string>::const_iterator name, end;
	
	for (size_t i = 0; i < servers.size(); ++i) {
		serv_name = servers[i]->getDirectives().find(NAME_DIRECTIVE);
		not_found = servers[i]->getDirectives().end();
		if (serv_name != not_found)
		{
			for (name = serv_name->second.begin(), end = serv_name->second.end(); name != end; ++name)
				if (*name == host)
					return servers[i];
		}
	}
	return servers.at(0);
}

static const Context* __get_match_location_context_(const std::vector<Context*> &locations, const std::string &path)
{
	int		character;
	std::map<std::string, const Context*, std::greater<std::string> > locate;
	std::map<std::string, const Context*, std::greater<std::string> >::const_iterator begin, end;
	
	if (locations.empty())
		throw 403;
	for (size_t i = 0; i < locations.size(); ++i) {
		locate.insert(std::make_pair(locations[i]->getArgs()[0], locations[i]));
	}
	for (begin = locate.begin(), end = locate.end(); begin != end; ++begin)
	{
		if (begin->first == "/") {
			return begin->second;
		}
		character = HTTP::strcmp(path.c_str(), begin->first.c_str());
		if (character == 0 || character == '/')
			return begin->second;
	}

	throw 403;
}

static const Context* __get_location_context_(const Request *request, std::vector<const Context*> &servers) {
	const Context *server = __get_match_server_context_(servers, request->headers.find("Host")->second);
	return __get_match_location_context_(server->getChildren(), request->path);
}

const Context* HTTP::blockMatchAlgorithm(const Client* client, const Context* const configuration) {
    std::vector<const Context*> servers;

	const std::vector<Context*>& children = configuration->getChildren().at(0)->getChildren();
	for (size_t i = 0; i < children.size(); ++i) {
		if (children[i]->getName() == SERVER_CONTEXT) {
			isValidServer(children[i], client, servers);
		}
	}

    return __get_location_context_(client->getRequest(), servers);
}
