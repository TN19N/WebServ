# include <string>
# include <vector>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <iostream>
# include <string.h>

# include "webserv/loadConfiguration.hpp"
# include "webserv/request.hpp"
# include "webserv/client.hpp"
# include "webserv/context.hpp"
# include "webserv/http.hpp"

static void isValidServer(const Context* context,  const Client* client, std::vector<const Context*>& servers) {
    if (context->getName() == SERVER_CONTEXT) {
        const std::string host = context->getDirective("host").front();
        const std::string port = context->getDirective("port").front();

        const struct sockaddr_storage& addr = client->getPeer();

        struct addrinfo *res;

        if (getaddrinfo(host.c_str(), port.c_str(), NULL, &res)  == -1) {
            std::cerr << "webserv: warning: getaddrinfo: " << strerror(errno) << std::endl;
            throw 500;
        }

        struct sockaddr_storage serverAddr = *(struct sockaddr_storage*)res->ai_addr;    

        if (addr.ss_family == serverAddr.ss_family) {
            if (addr.ss_family == AF_INET) {
                struct sockaddr_in* addr4 = (struct sockaddr_in*)&addr;
                struct sockaddr_in* serverAddr4 = (struct sockaddr_in*)&serverAddr;
                if (addr4->sin_port == serverAddr4->sin_port) {
                    if (addr4->sin_addr.s_addr == serverAddr4->sin_addr.s_addr || serverAddr4->sin_addr.s_addr == INADDR_ANY) {
                        servers.push_back(context);
                    }
                }
            } else if (addr.ss_family == AF_INET6) {
                struct sockaddr_in6* addr6 = (struct sockaddr_in6*)&addr;
                struct sockaddr_in6* serverAddr6 = (struct sockaddr_in6*)&serverAddr;
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

const Context* HTTP::blockMatchAlgorithm(Client* client, const Request* request, const Context* const configuration) {
    std::vector<const Context*> servers;

    for (std::vector<Context*>::const_iterator it = configuration->getChildren().begin(); it != configuration->getChildren().end(); ++it) {
        isValidServer(*it, client, servers);
    }

    return nullptr;
}