# include <cstddef>
# include <sys/socket.h>
# ifdef DEBUG
# include <iostream>
# endif

# include "webserv/client.hpp"
# include "webserv/core.hpp"

// * Constructor ****************************************************************************************************
Client::Client(const int *fd, const struct sockaddr_storage& clientAddr, const struct sockaddr* peerAddr, Client* cgiToClient)
    : fd(*fd), 
    clientAddr(clientAddr),
    peerAddr(peerAddr),
    request(nullptr),
    response(nullptr),
    cgiToClient(cgiToClient),
    clientToCgi(nullptr),
    type(cgiToClient == nullptr ? USER_CLIENT : CGI_CLIENT)
{
    if (cgiToClient != nullptr) {
        this->pipeFd[READ_END] = fd[READ_END];
        this->pipeFd[WRITE_END] = fd[WRITE_END];

        cgiToClient->setClientToCgi(this);
    }

    # ifdef DEBUG
    std::cout << "-------------------------- new Client --------------------------" << std::endl;
    std::cout << "client type: " << (this->getType() == USER_CLIENT ? "USER_CLIENT" : "CGI_CLIENT") << std::endl;
    if (this->type == USER_CLIENT) {
        CORE::display(reinterpret_cast<const struct sockaddr*>(&this->getClientAddr()));
        std::cout << "connected to: " << std::endl;
        CORE::display(this->getPeerAddr());
    } else if (this->type == CGI_CLIENT) {
        std::cout << "created to handle client: " << std::endl;
        CORE::display(reinterpret_cast<const struct sockaddr*>(&(this->getCgiToClient()->getClientAddr())));
    }
    # endif
}
// ******************************************************************************************************************

// * Getters ********************************************************************************************************
const int& Client::getFd() const {
    return this->fd;
}

const struct sockaddr_storage& Client::getClientAddr() const {
    return this->clientAddr;
}

const struct sockaddr* Client::getPeerAddr() const {
    return this->peerAddr;
}

const int* Client::getPipeFd() const {
    return this->pipeFd;
}

const Request* Client::getRequest() const {
    return this->request;
}

const Response* Client::getResponse() const {
    return this->response;
}

Request* Client::getRequest() {
    return this->request;
}

Response* Client::getResponse() {
    return this->response;
}

bool Client::getType() const {
    return this->type;
}

const Client* Client::getCgiToClient() const {
    return this->cgiToClient;
}

const Client* Client::getClientToCgi() const {
    return this->clientToCgi;
}

const std::string& Client::getBuffer() const {
    return this->buffer;
}
// ******************************************************************************************************************

// * Setters ********************************************************************************************************
void Client::setClientToCgi(Client* clientToCgi) {
    this->clientToCgi = clientToCgi;
}
// ******************************************************************************************************************

// * Destructor *****************************************************************************************************
Client::~Client() {
    # ifdef DEBUG
    std::cout << "-------------------------- client disconnected --------------------------" << std::endl;
    std::cout << "client type: " << (this->getType() == USER_CLIENT ? "USER_CLIENT" : "CGI_CLIENT") << std::endl;
    if (this->getType() == USER_CLIENT) {
        CORE::display(reinterpret_cast<const struct sockaddr*>(&(this->getClientAddr())));
    } else if (this->getType() == CGI_CLIENT) {
        std::cout << "cgiToClient: " << std::endl;
        CORE::display(reinterpret_cast<const struct sockaddr*>(&(this->getCgiToClient()->getClientAddr())));
    }
    # endif
}
// ******************************************************************************************************************