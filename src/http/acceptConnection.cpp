# include <sys/socket.h>
# include <vector>
# include <poll.h>
# include <iostream>
# include <fcntl.h>
# include <unistd.h>

# include "webserv/client.hpp"
# include "webserv/http.hpp"

# ifdef DEBUG
# include <arpa/inet.h>
# endif

void HTTP::acceptConnection(int listenFd, std::vector<pollfd>& fds, std::vector<Client*>& clients) {
    int clientFd = -1;
    
    struct sockaddr_storage clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    struct sockaddr_storage peerAddr;
    socklen_t peerAddrLen = sizeof(peerAddr);

    if (getsockname(listenFd, (struct sockaddr*)&peerAddr, &peerAddrLen) == -1) {
        std::cerr << "webserv: warning: getsockname() : " << strerror(errno) << std::endl;
        return;
    }

    if ((clientFd = accept(listenFd, (struct sockaddr*)&clientAddr, &clientAddrLen)) == -1) {
        std::cerr << "webserv: warning: accept() : " << strerror(errno) << std::endl;
        return;
    }

    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) {
        close(clientFd);
        std::cerr << "webserv: warning: fcntl() : " << strerror(errno) << std::endl;
        return;
    }

    struct pollfd clientPollFd;
    clientPollFd.fd = clientFd;
    clientPollFd.events = POLLIN;
    clientPollFd.revents = 0;
    fds.push_back(clientPollFd);

    clients.push_back(new Client(clientFd, clientAddr, peerAddr));

    # ifdef DEBUG
    std::cerr << " --------------------- new client --------------------- " << std::endl;
    std::cerr << "client fd: " << clientFd << std::endl;
    if (clientAddr.ss_family == AF_INET) {
        struct sockaddr_in* addr = (struct sockaddr_in*)&clientAddr;
        char ip[INET_ADDRSTRLEN];
        std::cerr << "sin_len: " << int(addr->sin_len) << std::endl;
        std::cerr << "sin_family: " << int(addr->sin_family) << std::endl;
        std::cerr << "sin_port: " << ntohs(addr->sin_port) << std::endl;
        std::cerr << "sin_addr: " << inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN) << std::endl;
    } else if (clientAddr.ss_family == AF_INET6) {
        struct sockaddr_in6* addr = (struct sockaddr_in6*)&clientAddr;
        char ip[INET6_ADDRSTRLEN];
        std::cerr << "sin6_len: " << int(addr->sin6_len) << std::endl;
        std::cerr << "sin6_family: " << int(addr->sin6_family) << std::endl;
        std::cerr << "sin6_port: " << ntohs(addr->sin6_port) << std::endl;
        std::cerr << "sin6_flowinfo: " << int(addr->sin6_flowinfo) << std::endl;
        std::cerr << "sin6_addr: " << inet_ntop(AF_INET6, &addr->sin6_addr, ip, INET6_ADDRSTRLEN) << std::endl;
        std::cerr << "sin6_scope_id: " << int(addr->sin6_scope_id) << std::endl;
    }
    std::cerr << "Connected to: " << std::endl;
    if (peerAddr.ss_family == AF_INET) {
        struct sockaddr_in* addr = (struct sockaddr_in*)&peerAddr;
        char ip[INET_ADDRSTRLEN];
        std::cerr << "sin_len: " << int(addr->sin_len) << std::endl;
        std::cerr << "sin_family: " << int(addr->sin_family) << std::endl;
        std::cerr << "sin_port: " << ntohs(addr->sin_port) << std::endl;
        std::cerr << "sin_addr: " << inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN) << std::endl;
    } else if (peerAddr.ss_family == AF_INET6) {
        struct sockaddr_in6* addr = (struct sockaddr_in6*)&peerAddr;
        char ip[INET6_ADDRSTRLEN];
        std::cerr << "sin6_len: " << int(addr->sin6_len) << std::endl;
        std::cerr << "sin6_family: " << int(addr->sin6_family) << std::endl;
        std::cerr << "sin6_port: " << ntohs(addr->sin6_port) << std::endl;
        std::cerr << "sin6_flowinfo: " << int(addr->sin6_flowinfo) << std::endl;
        std::cerr << "sin6_addr: " << inet_ntop(AF_INET6, &addr->sin6_addr, ip, INET6_ADDRSTRLEN) << std::endl;
        std::cerr << "sin6_scope_id: " << int(addr->sin6_scope_id) << std::endl;
    }
    std::cerr << " ------------------------------------------------------ " << std::endl;
    # endif
}