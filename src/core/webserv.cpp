# include <string>
# include <poll.h>
# include <iostream>
# include <vector>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <map>
# include <set>
# include <algorithm>

# include "../../include/webserv/webserv.hpp"
# include "../../include/webserv/core.hpp"
# include "../../include/webserv/context.hpp"
# include "../../include/webserv/configuration.hpp"
# include "../../include/webserv/http.hpp"
# include "../../include/webserv/client.hpp"

# define BIND_REPETITION 5

# define GREEN "\033[32m"
# define RESET "\033[0m"

// * Constructor ***********************************************************************************************************************************************************************
bool Webserv::webservState = WEB_SERV_RUNNING;

Webserv::Webserv(const std::string& configFilePath) 
    : configuration(CORE::loadConfiguration(configFilePath, new Context(MAIN_CONTEXT), 0))
{
    # ifdef DEBUG
    std::cout << "-------------------------- Configuration --------------------------" << std::endl;
    CORE::display(this->configuration, 0);
    # endif
}
// *************************************************************************************************************************************************************************************

// * Static Tools **********************************************************************************************************************************************************************
static bool isInRange(const struct sockaddr* addr, const std::vector<struct addrinfo*>& data) {
    for (int i = 0; i < data.size(); ++i) {
        if (addr->sa_family == data[i]->ai_addr->sa_family) {
            if (addr->sa_family == AF_INET) {
                const struct sockaddr_in* addr4 = reinterpret_cast<const struct sockaddr_in*>(addr);
                const struct sockaddr_in* serverAddr4 = reinterpret_cast<const struct sockaddr_in*>(data[i]->ai_addr);

                if (addr4->sin_addr.s_addr == serverAddr4->sin_addr.s_addr && addr4->sin_port == serverAddr4->sin_port) {
                    return true;
                }
            } else if (addr->sa_family == AF_INET6) {
                const struct sockaddr_in6* addr6 = reinterpret_cast<const struct sockaddr_in6*>(addr);
                const struct sockaddr_in6* serverAddr6 = reinterpret_cast<const struct sockaddr_in6*>(data[i]->ai_addr);

                if (memcmp(&(addr6->sin6_addr), &(serverAddr6->sin6_addr), sizeof(addr6->sin6_addr)) == 0 && addr6->sin6_port == serverAddr6->sin6_port) {
                    return true;
                }
            }
        }
    }
    return false;
}

static bool errorHandler(const int statusCode, Client* client) {
    if (client->getState() == SENDING_RESPONSE) {
        return true;
    } else {
        client->setResponse(new Response(statusCode, KEEP_ALIVE));
        client->getResponse()->addBody(HTTP::getDefaultErrorPage(statusCode));
        client->switchState();
        return false;
    }
}

static void redirectTo(const std::pair<int, std::string>& redirect, Client* client) {
    client->setResponse(new Response(redirect.first, CLOSE_CONNECTION));
    client->getResponse()->addHeader("Location", redirect.second);
    client->getResponse()->buffer += "\r\n";
    client->switchState();
}
// *************************************************************************************************************************************************************************************

// * Methods ***************************************************************************************************************************************************************************
void Webserv::startServers() {
    const std::vector<Context*>& contexts = this->configuration->getChildren();
    std::vector<struct addrinfo*> serversAddress;

    for (std::vector<Context*>::const_iterator context = contexts.begin(); context != contexts.end(); ++context) {
        if ((*context)->getName() == HTTP_CONTEXT) {
            const std::vector<Context*>& contexts = (*context)->getChildren();
			for (std::vector<Context*>::const_iterator context = contexts.begin(); context != contexts.end(); ++context) {
                if ((*context)->getName() == SERVER_CONTEXT) {
                    struct addrinfo hints, *res = NULL;
                    int sockfd;

                    memset(&hints, 0, sizeof(hints));
                    hints.ai_family = AF_UNSPEC;
                    hints.ai_socktype = SOCK_STREAM;

                    const std::string& port = (*context)->getDirective(PORT_DIRECTIVE).at(0);
                    const std::string& host = (*context)->getDirective(HOST_DIRECTIVE).at(0);
					
                    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0 || res == NULL) {
                        throw std::runtime_error("getaddrinfo() : " + std::string(gai_strerror(errno)));
                    }

                    if (res->ai_family == AF_INET6) {
                        struct addrinfo* tmp = res;
                        while (tmp->ai_next != NULL && tmp->ai_next->ai_family != AF_INET) {
                            tmp = tmp->ai_next;
                        }
                        if (tmp->ai_next != NULL) {
                            struct addrinfo* tmp2 = tmp->ai_next;
                            tmp->ai_next = tmp->ai_next->ai_next;
                            tmp2->ai_next = res;
                            res = tmp2;
                        }
                    }


                    if (isInRange(res->ai_addr, serversAddress)) {
                        freeaddrinfo(res->ai_next);
						serversAddress.push_back(res);
						continue;
                    }
					serversAddress.push_back(res);
	
					if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
                        freeaddrinfo(res->ai_next);
                        throw std::runtime_error("socket() : " + std::string(strerror(errno)));
                    }

                    int yes = 1;
                    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
                        close(sockfd);
                        freeaddrinfo(res->ai_next);
                        throw std::runtime_error("setsockopt() : " + std::string(strerror(errno)));
                    }

                    size_t  repetition = BIND_REPETITION;
                    while (true) {
                        if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
                            if (errno == EADDRINUSE && repetition --> 0) {
                                std::cerr << "Address (" << host + ":" + port << ") already in use, retrying in 1 second..." << std::endl;
                                sleep(1);
                            } else {
                                close(sockfd);
                                freeaddrinfo(res->ai_next);
                                throw std::runtime_error("bind() : " + std::string(strerror(errno)));
                            }
                        } else {
                            break;
                        }
                    }

                    if (listen(sockfd, SOMAXCONN) == -1) {
                        close(sockfd);
                        freeaddrinfo(res->ai_next);
                        throw std::runtime_error("listen() : " + std::string(strerror(errno)));
                    }
					
                    this->serversSocketFd.push_back(sockfd);

                    # ifdef DEBUG
                    std::cout << "-------------------------- new Server --------------------------" << std::endl;
                    CORE::display(res);
                    # endif

                    freeaddrinfo(res->ai_next);
                }
            }
        }
    }

    for (size_t i = 0; i < serversAddress.size(); ++i) {
        serversAddress[i]->ai_next = NULL;
        freeaddrinfo(serversAddress[i]);
    }
}

void Webserv::removeClient(const Client* client) {
    this->clients.erase(std::find(this->clients.begin(), this->clients.end(), client));
    delete client;
}

void Webserv::run() {
    try {
        Webserv::startServers();
        CORE::listenToSignals();
    } catch (const std::exception& e) {
        throw std::runtime_error("initialization failed : " + std::string(e.what()));
    }

    std::cerr << GREEN;
    std::cerr << "**********************************" << std::endl;
    std::cerr << "* Webserv: webserv is running... *"<< std::endl;
    std::cerr << "**********************************" << std::endl;
    std::cerr << RESET;

    int    pollResult = 0;
    while (Webserv::webservState == WEB_SERV_RUNNING) {
        std::vector<pollfd> fds = CORE::fillFds(this->serversSocketFd, this->clients);

        if ((pollResult = poll(fds.data(), fds.size(), -1)) == -1) {
            if (errno == EINTR) {
                continue;
            }
            throw std::runtime_error("poll() : " + std::string(strerror(errno)));
        }

        for (int i = 0; i < fds.size() && pollResult > 0; ++i) {
            try {
                --pollResult;
                if (fds[i].revents & POLLHUP) {
                    Webserv::removeClient(HTTP::getClientWithFd(fds[i].fd, this->clients));
                } else if (fds[i].revents & POLLIN) {
                    if (i < this->serversSocketFd.size()) {
                        HTTP::acceptConnection(fds[i].fd, this->clients);
                    } else {
                        HTTP::requestHandler(HTTP::getClientWithFd(fds[i].fd, this->clients), this->configuration);
                    }
                } else if (fds[i].revents & POLLOUT) {
					Client *client = HTTP::getClientWithFd(fds[i].fd, this->clients);
					if (HTTP::responseHandler(client) == false) {
                        Webserv::removeClient(HTTP::getClientWithFd(fds[i].fd, this->clients));
                    }
                } else {
                    ++pollResult;
                }
            } catch (const int statusCode) {
                if (errorHandler(statusCode, HTTP::getClientWithFd(fds[i].fd, this->clients))) {
                    Webserv::removeClient(HTTP::getClientWithFd(fds[i].fd, this->clients));
                }
            } catch (const std::pair<int, std::string>& redirect) {
                redirectTo(redirect, HTTP::getClientWithFd(fds[i].fd, this->clients));
            } catch (const std::exception& e) {
                if (errorHandler(500, HTTP::getClientWithFd(fds[i].fd, this->clients))) {
                    Webserv::removeClient(HTTP::getClientWithFd(fds[i].fd, this->clients));
                }
            }
        }
    }
}
// *************************************************************************************************************************************************************************************

// * Destructor ************************************************************************************************************************************************************************
Webserv::~Webserv() {
    for (size_t i = 0; i < this->serversSocketFd.size(); ++i) {
        close(this->serversSocketFd[i]);
    }
	for (size_t i = 0; i < this->clients.size(); ++i)
		delete this->clients[i] ;

    delete this->configuration;
}
// *************************************************************************************************************************************************************************************