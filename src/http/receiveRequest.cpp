# define BUFFER_SIZE 4096

# include <iostream>
# include <string>
# include <vector>
# include <iostream>

# include "webserv/client.hpp"

# ifdef DEBUG
# include <arpa/inet.h>
# endif

void receiveRequest(Client* client, std::vector<int>& fdsToClose) {
    char buffer[BUFFER_SIZE];
    int bytesReceived = recv(client->getFd(), buffer, BUFFER_SIZE, 0);
    if (bytesReceived == -1) {
        std::cerr << "Error: recv() failed: " << strerror(errno) << std::endl;
    } else if (bytesReceived == 0) {
        fdsToClose.push_back(client->getFd());
        # ifdef DEBUG
        std::cerr << " --------------------- client closed --------------------- " << std::endl;
        std::cerr << "client fd: " << client->getFd() << std::endl;
        struct sockaddr_storage clientAddr = client->getAddr();
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
        # endif
    } else {
        client->addBuffer(buffer, bytesReceived);
        if (client->getRequest() == nullptr && client->getBuffer().find("\r\n\r\n") != std::string::npos) {
            client->newRequest();
        }
    }
}