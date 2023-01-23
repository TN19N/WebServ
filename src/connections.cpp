# include <sys/socket.h>
# include <arpa/inet.h>
# include <iostream>
# include <algorithm>
# include <unistd.h>

# include "definitions.hpp"
# include "webServ.hpp"

void acceptConnection(const int& sockfd) {
    struct sockaddr_storage client_addr;
    socklen_t addr_size = sizeof(client_addr);
    int client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &addr_size);
    if (client_fd == -1) {
        std::cerr << "webserv: acceptConnection() : accept() : " << strerror(errno) << std::endl;
        return;
    }

    struct pollfd client;
    client.fd = client_fd;
    client.events = POLLIN;
    WebServ::pollFds.push_back(client);

    # ifdef DEBUG_ACCEPT_CONNECTION
        std::cerr << " ------------------------------------------ " << std::endl;
        std::cerr << "DEBUG_ACCEPT_CONNECTION:" << std::endl;
        std::cerr << " ------------------------------------------ " << std::endl;
        std::cerr << "client fd : " << client_fd << std::endl;
        std::cerr << "addrinfo:: " << std::endl;
        std::cerr << "  family : " << int(client_addr.ss_family) << std::endl;
        if (client_addr.ss_family == AF_INET) {
            struct sockaddr_in* addr = (struct sockaddr_in*)&client_addr;
            std::cerr << "  port : " << ntohs(addr->sin_port) << std::endl;
            std::cerr << "  addr : " << inet_ntoa(addr->sin_addr) << std::endl;
        } else if (client_addr.ss_family == AF_INET6) {
            struct sockaddr_in6* addr = (struct sockaddr_in6*)&client_addr;
            std::cerr << "  port : " << ntohs(addr->sin6_port) << std::endl;
            char ip6[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &addr->sin6_addr, ip6, INET6_ADDRSTRLEN);
            std::cerr << "  addr : " << ip6 << std::endl;
        }
    # endif
}

static void closeConnection(const int& fd) {
    # ifdef DEBUG_CLOSE_CONNECTION
        std::cerr << " ------------------------------------------ " << std::endl;
        std::cerr << "DEBUG_CLOSE_CONNECTION:" << std::endl;
        std::cerr << " ------------------------------------------ " << std::endl;
        std::cerr << "client fd : " << fd << std::endl;
    # endif
    close(fd);
    for (size_t i = 0; i < WebServ::pollFds.size(); ++i) {
        if (WebServ::pollFds[i].fd == fd) {
            WebServ::pollFds.erase(WebServ::pollFds.begin() + i);
            break;
        }
    }
}

void readRequest(const int& fd) {
    std::string request;
    char buf[READ_BUF_SIZE];

    int bytes_read = recv(fd, buf, READ_BUF_SIZE, 0);
    while (bytes_read > 0) {
        std::cerr << "====> " << int(buf[bytes_read - 1]) << std::endl;
        request.append(buf, bytes_read);
        bytes_read = recv(fd, buf, READ_BUF_SIZE, 0);
    }

    # ifdef DEBUG_READ_REQUEST
        std::cerr << " ------------------------------------------ " << std::endl;
        std::cerr << "DEBUG_READ_REQUEST:" << std::endl;
        std::cerr << " ------------------------------------------ " << std::endl;
        std::cerr << "client fd : " << fd << std::endl;
        std::cerr << "request : " << std::endl;
        std::cerr << request << std::endl;
    # endif

    std::cerr << "[[" << request.back() << "]]" << std::endl;

    if (bytes_read == -1) {
        std::cerr << "webserv: readRequest() : recv() : " << strerror(errno) << std::endl;
        return;
    }

    if (bytes_read == 0) {
        closeConnection(fd);
    }

    # ifdef DEBUG_READ_REQUEST
        std::cerr << " ------------------------------------------ " << std::endl;
        std::cerr << "DEBUG_READ_REQUEST:" << std::endl;
        std::cerr << " ------------------------------------------ " << std::endl;
        std::cerr << "client fd : " << fd << std::endl;
        std::cerr << "request : " << std::endl;
        std::cerr << request << std::endl;
    # endif
}