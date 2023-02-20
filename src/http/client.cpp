# include "webserv/client.hpp"

Client::Client(const int fd, const struct sockaddr_storage& addr)
    : fd(fd), 
    addr(addr),
    request(nullptr)
{}

const int& Client::getFd() const {
    return this->fd;
}

const struct sockaddr_storage& Client::getAddr() const {
    return this->addr;
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