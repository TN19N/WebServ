# include <vector>
# include <poll.h>
# include <sys/socket.h>
# include <iostream>
# include <fcntl.h>
# include <unistd.h>

# include "../../include/webserv/http.hpp"
# include "../../include/webserv/client.hpp"
# include "../../include/webserv/core.hpp"

void HTTP::acceptConnection(const int serverFd, std::vector<Client*>& clients) {
    int clientFd = 0;

    struct sockaddr_storage clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    struct sockaddr_storage peerAddr;
    socklen_t peerAddrLen = sizeof(peerAddr);

    if (getsockname(serverFd, reinterpret_cast<struct sockaddr*>(&peerAddr), &peerAddrLen) == -1) {
        std::cerr << "webserv: getsockname(): " << strerror(errno) << std::endl;
        throw 500;
    }

    if ((clientFd = accept(serverFd, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen)) == -1) {
        std::cerr << "webserv: accept(): " << strerror(errno) << std::endl;
        return ;
    }

    clients.push_back(new Client(&clientFd, clientAddr, peerAddr));

    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) {
        std::cerr << "webserv: fcntl(): " << strerror(errno) << std::endl;
        throw 500;
    }
}