# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <poll.h>
# include <string.h>
# include <vector>
# include <unistd.h>
# include <sys/errno.h>
# include <iostream>
# include <fcntl.h>

# ifdef DEBUG
# include <arpa/inet.h>
# endif

# include "webserv/context.hpp"
# include "webserv/core.hpp"

# define BACKLOG 25

static void startServers(const Context* context, std::vector<pollfd>& fds, size_t& serversCounter) {
    if (context->getName() == SERVER_CONTEXT) {
        struct addrinfo hints, *res, *ptr;
        int             status, sockfd;
        struct pollfd   fd;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        const std::string& port = context->getDirective(PORT_DIRECTIVE).front();
        const std::string& host = context->getDirective(HOST_DIRECTIVE).front();

        if ((status = getaddrinfo(host.c_str(), port.c_str(), &hints, &res)) != 0) {
            throw std::runtime_error("getaddrinfo: " + std::string(gai_strerror(status)));
        }
        for (ptr = res; ptr != NULL; ptr = ptr->ai_next) {
            if ((sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1) {
                continue;
            }
            int yes = 1;
            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
                close(sockfd);
                freeaddrinfo(res);
                throw std::runtime_error("setsockopt: " + std::string(strerror(errno)));
            }
            if (bind(sockfd, ptr->ai_addr, ptr->ai_addrlen) == -1) {
                close(sockfd);
                std::cerr << "webserv: warning: bind: " << strerror(errno) << std::endl;
                continue;
            }
            break;
        }

        if (ptr != NULL) {
            struct pollfd fd;
            fd.fd = sockfd;
            fd.events = POLLIN;
            fd.revents = 0;
            fds.push_back(fd);
            ++serversCounter;

            if (listen(sockfd, BACKLOG) == -1) {
                freeaddrinfo(res);
                throw std::runtime_error("listen: " + std::string(strerror(errno)));
            }

            if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1) {
                freeaddrinfo(res);
                throw std::runtime_error("fcntl: " + std::string(strerror(errno)));
            }

            # ifdef DEBUG
            std::cerr << " ---------------------------------------- SERVER ---------------------------------------- " << std::endl;
            std::cerr << "ai_flags: " << int(ptr->ai_flags) << std::endl;
            std::cerr << "ai_family: " << int(ptr->ai_family) << std::endl;
            std::cerr << "ai_socktype: " << int(ptr->ai_socktype) << std::endl;
            std::cerr << "ai_protocol: " << int(ptr->ai_protocol) << std::endl;
            std::cerr << "ai_addrlen: " << int(ptr->ai_addrlen) << std::endl;
            std::cerr << "ai_canonname: " << (ptr->ai_canonname == NULL ? "NULL" : ptr->ai_canonname) << std::endl;
            std::cerr << "ai_addr: " << std::endl;
            if (ptr->ai_family == AF_INET) {
                struct sockaddr_in* addr = reinterpret_cast<struct sockaddr_in*>(ptr->ai_addr);
                char ip[INET_ADDRSTRLEN];
                std::cerr << "    sin_len: " << int(addr->sin_len) << std::endl;
                std::cerr << "    sin_family: " << int(addr->sin_family) << std::endl;
                std::cerr << "    sin_port: " << ntohs(addr->sin_port) << std::endl;
                std::cerr << "    sin_addr: " << inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN) << std::endl;
            } else if (ptr->ai_family == AF_INET6) {
                struct sockaddr_in6* addr = reinterpret_cast<struct sockaddr_in6*>(ptr->ai_addr);
                char ip[INET6_ADDRSTRLEN];
                std::cerr << "    sin6_len: " << int(addr->sin6_len) << std::endl;
                std::cerr << "    sin6_family: " << int(addr->sin6_family) << std::endl;
                std::cerr << "    sin6_port: " << ntohs(addr->sin6_port) << std::endl;
                std::cerr << "    sin6_flowinfo: " << int(addr->sin6_flowinfo) << std::endl;
                std::cerr << "    sin6_addr: " << inet_ntop(AF_INET6, &addr->sin6_addr, ip, INET6_ADDRSTRLEN) << std::endl;
                std::cerr << "    sin6_scope_id: " << int(addr->sin6_scope_id) << std::endl;
            }
            # endif
        } else {
            # ifdef DEBUG
            std::cerr << " ---------------------------------------- SERVER ---------------------------------------- " << std::endl;
            std::cerr << "host: " << host << std::endl;
            std::cerr << "port: " << port << std::endl;
            # endif
        }

        if (res != NULL) {
            freeaddrinfo(res);
        }
    }

    for (std::vector<Context*>::const_iterator it = context->getChildren().begin(); it != context->getChildren().end(); ++it) {
        startServers(*it, fds, serversCounter);
    }
}

size_t CORE::init(const Context* configuration, std::vector<pollfd>& fds) {
    size_t serversCounter = 0;

    const std::vector<Context*>& children = configuration->getChildren();
    for (std::vector<Context*>::const_iterator it = children.begin(); it != children.end(); ++it) {
        startServers(*it, fds, serversCounter);
    }

    if (fds.empty() == true) {
        throw std::runtime_error("No server to start");
    }

    return serversCounter;
}