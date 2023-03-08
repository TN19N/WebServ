# include <cstddef>
# include <sys/socket.h>
# include <string>
# include <vector>
# include <poll.h>
# include <unistd.h>
# ifdef DEBUG
# include <iostream>
# endif

# include "../../include/webserv/client.hpp"
# include "../../include/webserv/core.hpp"
# include "../../include/webserv/http.hpp"

// * Constructor ****************************************************************************************************
Client::Client(const int *fd, const struct sockaddr_storage& clientAddr, const struct sockaddr_storage& peerAddr, Client* cgiToClient)
    : socketFd(*fd),
    clientAddr(clientAddr),
    peerAddr(peerAddr),
    request(cgiToClient == nullptr ? nullptr : cgiToClient->getRequest()),
    response(nullptr),
    cgiToClient(cgiToClient),
    clientToCgi(nullptr),
    state(cgiToClient == nullptr ? RIDING_REQUEST : SENDING_REQUEST)
{
    if (cgiToClient != nullptr) {
        this->pipeFd[READ_END] = fd[READ_END];
        this->pipeFd[WRITE_END] = fd[WRITE_END];

        cgiToClient->setClientToCgi(this);
    }

    # ifdef DEBUG
    std::cout << "-------------------------- new Client --------------------------" << std::endl;
    std::cout << "client type: " << (this->isCgi() ? "CGI_CLIENT" : "USER_CLIENT") << std::endl;
    if (this->isCgi()) {
        std::cout << "created to handle client: " << std::endl;
        CORE::display(reinterpret_cast<const struct sockaddr*>(&(this->getCgiToClient()->getClientAddr())));
    } else {
        CORE::display(reinterpret_cast<const struct sockaddr*>(&this->getClientAddr()));
        std::cout << "connected to: " << std::endl;
        CORE::display(reinterpret_cast<const struct sockaddr*>(&this->getPeerAddr()));
    }
    # endif
}
// ******************************************************************************************************************

// * Getters ********************************************************************************************************
const int Client::getSocketFd() const {
    return this->socketFd;
}

const struct sockaddr_storage& Client::getClientAddr() const {
    return this->clientAddr;
}

const struct sockaddr_storage& Client::getPeerAddr() const {
    return this->peerAddr;
}

const int* Client::getPipeFd() const {
    return this->pipeFd;
}

int* Client::getPipeFd() {
    return this->pipeFd;
}

void Client::newRequest(Request *req) {
	this->request = req;
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

const Client* Client::getCgiToClient() const {
    return this->cgiToClient;
}

const Client* Client::getClientToCgi() const {
    return this->clientToCgi;
}

const std::string& Client::getBuffer() const {
    return this->buffer;
}

std::string& Client::getBuffer() {
    return this->buffer;
}

const int Client::getState() const {
    return this->state;
}

const int Client::getFdOf(const int index) const {
    if (this->isCgi()) {
        return this->getPipeFd()[index];
    }
    return this->getSocketFd();
}
// ******************************************************************************************************************

// * Setters ********************************************************************************************************
void Client::setClientToCgi(Client* clientToCgi) {
    this->clientToCgi = clientToCgi;
}

void Client::setRequest(Request* request) {
    this->request = request;
}

void Client::setResponse(Response* response) {
    this->response = response;
}

void Client::setState(const bool& state) {
    this->state = state;
}
// ******************************************************************************************************************

// * Methods ********************************************************************************************************
void Client::switchState() {
    switch (this->getState()) {
        case RIDING_REQUEST :
            this->setState(SENDING_RESPONSE);
            delete this->request;
            this->request = nullptr;
            break;
        case SENDING_RESPONSE :
            this->setState(SENDING_RESPONSE);
            delete this->response;
            this->response = nullptr;
            break;
        case SENDING_REQUEST :
            close(this->getFdOf(READ_END));
            this->setState(RIDING_REQUEST);
            delete this->request;
            this->request = nullptr;
            break;
    }
}

const bool Client::isCgi() const {
    return this->getCgiToClient() != nullptr;
}
// ******************************************************************************************************************

// * Destructor *****************************************************************************************************
Client::~Client() {

    switch (this->getState()) {
        case RIDING_REQUEST :
        case SENDING_RESPONSE :
            close(this->getSocketFd());
            break;
        case SENDING_REQUEST :
            close(this->getPipeFd()[WRITE_END]);
        case RIDING_RESPONSE :
            close(this->getPipeFd()[READ_END]);
    }

    if (this->getRequest() != nullptr) {
        delete this->getRequest();
    }

    if (this->getResponse() != nullptr) {
        delete this->getResponse();
    }
    
    # ifdef DEBUG
    std::cout << "-------------------------- client disconnected --------------------------" << std::endl;
    std::cout << "client type: " << (this->isCgi() ? "CGI_CLIENT" : "USER_CLIENT") << std::endl;
    if (this->isCgi()) {
        std::cout << "cgiToClient: " << std::endl;
        CORE::display(reinterpret_cast<const struct sockaddr*>(&(this->getCgiToClient()->getClientAddr())));
    } else {
        CORE::display(reinterpret_cast<const struct sockaddr*>(&(this->getClientAddr())));
    }
    # endif
}
// ******************************************************************************************************************