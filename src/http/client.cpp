# include "webserv/client.hpp"

Client::Client(const int fd, const struct sockaddr_storage& addr) 
    : fd(fd), 
    addr(addr) 
{}

const int& Client::getFd() const {
    return fd;
}