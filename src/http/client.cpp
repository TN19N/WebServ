# include "webserv/client.hpp"

Client::Client(const int fd, const struct sockaddr_storage& addr, const struct sockaddr_storage& peer)
    : fd(fd), 
    addr(addr),
    peer(peer),
    request(nullptr)
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

const Request* Client::getRequest() const {
    return this->request;
}

void Client::newRequest() {
    
}

Client::~Client() {
}