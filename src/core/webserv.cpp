# include <stdexcept>
# include <string>
# include <poll.h>
# include <iostream>
# include <unistd.h>

# include "webserv/webserv.hpp"
# include "webserv/core.hpp"
# include "webserv/context.hpp"
# include "webserv/http.hpp"

# ifdef DEBUG
static void display(const Context* context, const size_t level) {
    const std::vector<std::string>& args = context->getArgs();
    std::cerr << std::string(level, '\t');
    for (size_t i = 0; i < args.size(); ++i) {
        std::cerr << "[" << args[i] << "] ";
    }
    std::cerr << std::endl;

    const std::map<const std::string, std::vector<std::string> >& directives = context->getDirectives();
    for (std::map<const std::string, std::vector<std::string> >::const_iterator it = directives.begin(); it != directives.end(); ++it) {
        std::cerr << std::string(level + 1, '\t') << "(" << it->first << ") : ";
        for (size_t i = 0; i < it->second.size(); ++i) {
            std::cerr << "[" << it->second[i] << "] ";
        }
        std::cerr << std::endl;
    }

    for (std::vector<Context*>::const_iterator it = context->getChildren().begin(); it != context->getChildren().end(); ++it) {
        display(*it, level + 1);
    }
}
# endif

// static member initialization
WebServer* WebServer::instance = nullptr;

WebServer::WebServer(const std::string& configFilePath) 
    : configuration(CORE::loadConfiguration(configFilePath, nullptr, 1)) 
{
    # ifdef DEBUG
    std::cerr << " --------------------- configuration --------------------- " << std::endl;
    display(configuration, 0);
    # endif
}

WebServer& WebServer::getInstance(const std::string& configFilePath) {
    if (WebServer::instance == nullptr) {
        try {
            WebServer::instance = new WebServer(configFilePath);
        } catch (const std::exception& e) {
            throw std::runtime_error("loading configuration failed: " + std::string(e.what()));
        }
    }
    return *(WebServer::instance);
}

void WebServer::run() {
    size_t serversCount = 0;
    try {
        serversCount = CORE::init(this->configuration, this->fds);
    } catch (const std::exception& e) {
        delete this;
        throw std::runtime_error("servers initialization failed: " + std::string(e.what()));
    }

    int pollResult = -1;
    while (true) {
        std::vector<const Client*> clientsToRemove;
        if ((pollResult = poll(this->fds.data(), this->fds.size(), -1)) == -1) {
            delete this;
            throw std::runtime_error("poll failed '" + std::string(strerror(errno)) + "'");
        }

        for (size_t i = 0; i < this->fds.size() && pollResult > 0; ++i) {
            if (this->fds[i].revents & POLLIN) {
                --pollResult;
                if (i < serversCount) {
                    HTTP::acceptConnection(this->fds[i].fd, this->fds, this->clients);
                } else {
                    HTTP::requestHandler(this->clients[i - serversCount], this->configuration, clientsToRemove);
                }
            }
        }

        // close fds
        while (clientsToRemove.empty() == false) {
            for (std::vector<pollfd>::iterator it = this->fds.begin(); it != this->fds.end(); ++it) {
                if (it->fd == clientsToRemove.back()->getFd()) {
                    this->fds.erase(it);
                    break;
                }
            }
            for (std::vector<Client*>::iterator it = this->clients.begin(); it != this->clients.end(); ++it) {
                if ((*it) == clientsToRemove.back()) {
                    close((*it)->getFd());
                    delete (*it);
                    this->clients.erase(it);
                    break;
                }
            }
            clientsToRemove.pop_back();
        }
    }

    std::cout << "webserv stoped" << std::endl;
    delete this;
}

WebServer::~WebServer() {
    // clear clients
    for (std::vector<Client*>::iterator it = this->clients.begin(); it != this->clients.end(); ++it) {
        delete *it;
    }
    clients.clear();
    // close and clear poll fds
    for (std::vector<pollfd>::iterator it = this->fds.begin(); it != this->fds.end(); ++it) {
        close(it->fd);
    }
    fds.clear();
    delete this->configuration;
}