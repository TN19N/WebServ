# include <fcntl.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <unistd.h>
# include <iostream>

# include "definitions.hpp"
# include "webServ.hpp"
# include "server.hpp"

static void panic(const std::string& msg) {
    std::cerr << "webserv: init error : " << msg << std::endl;
    WebServ::clear();
    exit(EXIT_FAILURE);
}

void init() {

    # ifdef DEBUG_SERVER_INIT 
        std::cerr << " ------------------------------------------ " << std::endl;
        std::cerr << "DEBUG_SERVER_INIT:" << std::endl;
    # endif

    for (size_t i = 0; i < WebServ::servers.size(); ++i) {
        Server& server = WebServ::servers[i];

        struct addrinfo hints, *res, *ptr;
        int status;
        int sockfd;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        if ((status = getaddrinfo(server.getHost().c_str(), std::to_string(server.getPort()).c_str(), &hints, &res)) != 0) {
            panic(gai_strerror(status));
        }

        for (ptr = res; ptr != NULL; ptr = ptr->ai_next) {
            if ((sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1) {
                std::cerr << "webserv: init error : socket() : " << strerror(errno) << std::endl;
                continue;
            }

            if (bind(sockfd, ptr->ai_addr, ptr->ai_addrlen) == -1) {
                close(sockfd);
                std::cerr << "webserv: init error : bind() '" << server.getHost() + ":" + std::to_string(server.getPort()) 
                << "' : " << strerror(errno) << " (ignoring)." << std::endl;
                continue;
            }

            break;
        }

        if (ptr == NULL) {
            freeaddrinfo(res);
            continue;
        }

        if (listen(sockfd, BACKLOG) == -1) {
            freeaddrinfo(res);
            panic(std::string("listen() : ") + strerror(errno));
        }

        if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1) {
            freeaddrinfo(res);
            panic(std::string("fcntl() : ") + strerror(errno));
        }

        struct pollfd pollFd;
        pollFd.fd = sockfd;
        pollFd.events = POLLIN;
        WebServ::pollFds.push_back(pollFd);

        # ifdef DEBUG_SERVER_INIT
            std::cerr << " ------------------------------------------ " << std::endl;
            std::cerr << "sockfd: " << sockfd << std::endl;
            std::cerr << "host: " << server.getHost() << std::endl;
            std::cerr << "port: " << server.getPort() << std::endl;
            std::cerr << "addrinfo:: " << std::endl;
            std::cerr << "  ai_flags: " << ptr->ai_flags << std::endl;
            std::cerr << "  ai_family: " << ptr->ai_family << std::endl;
            std::cerr << "  ai_socktype: " << ptr->ai_socktype << std::endl;
            std::cerr << "  ai_protocol: " << ptr->ai_protocol << std::endl;
            std::cerr << "  ai_addrlen: " << ptr->ai_addrlen << std::endl;
            std::cerr << "  ai_canonname: " << (ptr->ai_canonname == NULL ? "NULL" : ptr->ai_canonname) << std::endl;
            std::cerr << "  ai_addr_in:: " << std::endl;
            std::cerr << "    sin_family: " << ((struct sockaddr_in*)ptr->ai_addr)->sin_family << std::endl;
            std::cerr << "    sin_port: " << ntohs(((struct sockaddr_in*)ptr->ai_addr)->sin_port) << std::endl;
            std::cerr << "    sin_addr.s_addr: " << inet_ntoa(((struct sockaddr_in*)ptr->ai_addr)->sin_addr) << std::endl;
        # endif

        freeaddrinfo(res);
    }

    if (WebServ::pollFds.empty() == true) {
        panic("failed to bind to any address/port.");
    }
}