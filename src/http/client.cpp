# include "webserv/client.hpp"
# include "webserv/request.hpp"

# ifdef DEBUG
# include <iostream>
# include <arpa/inet.h>
# endif

Client::Client(const int fd, const struct sockaddr_storage& addr, const struct sockaddr_storage& peer, const Client* cgiClient)
    : fd(fd), 
    addr(addr),
    peer(peer),
    request(nullptr),
    cgiClient(cgiClient)
{}

const int& Client::getFd() const {
    return this->fd;
}

const struct sockaddr_storage& Client::getAddr() const {
    return this->addr;
}
const struct sockaddr_storage& Client::getPeer() const {
    return this->peer;
}

const std::string& Client::getBuffer() const {
    return this->buffer;
}

void Client::addBuffer(char *buf, int len) {
    this->buffer.append(buf, len);
}

Request* Client::getRequest() const {
    return this->request;
}

void Client::newRequest(Request* request) {
    this->request = request;
}

Client::~Client() {
    # ifdef DEBUG
    std::cerr << " ----------------------- Client removed ----------------------- " << std::endl;
    std::cerr << "client fd: " << this->fd << std::endl;
    if (this->addr.ss_family == AF_INET) {
        struct sockaddr_in *s = (struct sockaddr_in *)&this->addr;
        char ip[INET_ADDRSTRLEN];
        std::cerr << "sin_family: " << int(s->sin_family) << std::endl;
        std::cerr << "sin_len: " << int(s->sin_len) << std::endl;
        std::cerr << "sin_port: " << ntohs(s->sin_port) << std::endl;
        std::cerr << "sin_addr: " << inet_ntop(AF_INET, &s->sin_addr, ip, INET_ADDRSTRLEN) << std::endl;
    } else if (this->addr.ss_family == AF_INET6) {
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&this->addr;
        char ip[INET6_ADDRSTRLEN];
        std::cerr << "sin6_family: " << int(s->sin6_family) << std::endl;
        std::cerr << "sin6_len: " << int(s->sin6_len) << std::endl;
        std::cerr << "sin6_port: " << ntohs(s->sin6_port) << std::endl;
        std::cerr << "sin6_addr: " << inet_ntop(AF_INET6, &s->sin6_addr, ip, INET6_ADDRSTRLEN) << std::endl;
        std::cerr << "sin6_flowinfo: " << s->sin6_flowinfo << std::endl;
        std::cerr << "sin6_scope_id: " << s->sin6_scope_id << std::endl;
    }
    # endif

    if (this->request != nullptr) {
        delete this->request;
    }
}