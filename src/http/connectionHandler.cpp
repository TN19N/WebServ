# include <vector>
# include <poll.h>
# include <sys/socket.h>
# include <iostream>
# include <fcntl.h>
# include <unistd.h>

# include "webserv/http.hpp"
# include "webserv/client.hpp"
# include "webserv/core.hpp"

void HTTP::acceptConnection(const int serverFd, std::vector<pollfd>& fds, std::vector<Client*>& clients, const struct sockaddr* serverAddr) {
    int clientFd = 0;
    struct sockaddr_storage clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    memset(&clientAddr, 0, sizeof(clientAddr));
    if ((clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &clientAddrLen)) == -1) {
        std::cerr << "webserv: accept(): " << strerror(errno) << std::endl;
        return ;
    }

    struct pollfd clientPollFd;
    clientPollFd.fd = clientFd;
    clientPollFd.events = POLLIN | POLLHUP;
    fds.push_back(clientPollFd);

    clients.push_back(new Client(&clientFd, clientAddr, serverAddr));

    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) {
        std::cerr << "webserv: fcntl(): " << strerror(errno) << std::endl;
        throw 500;
    }
}

void HTTP::closeConnection(std::vector<pollfd>& fds, const int index, std::vector<Client*>& clients) {
    close(fds[index].fd);
    fds.erase(fds.begin() + index);

    const Client* clientToRemove = CORE::getClientWithFd(fds[index].fd, clients);
    clients.erase(std::find(clients.begin(), clients.end(), clientToRemove));

    delete clientToRemove;
}