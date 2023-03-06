# include <string>
# include <vector>
# include <signal.h>
# include <sys/errno.h>
# ifdef DEBUG
# include <iostream>
# include <arpa/inet.h>
# include <netdb.h>
# endif

# include "webserv/core.hpp"
# include "webserv/webserv.hpp"
# include "webserv/client.hpp"

// * DEBUG ****************************************************************************************************************************
# ifdef DEBUG
void CORE::display(const Context* context, const size_t level) {
    std::cout << std::string(level, '\t') << "[" << context->getName() << "] :: ";

    const std::vector<std::string>& args = context->getArgs();
    for (size_t i = 0; i < args.size(); ++i) {
        std::cout << args[i] << " ";
    }
    std::cout << std::endl;

    for (std::map<std::string, std::vector<std::string> >::const_iterator it = context->getDirectives().begin(); it != context->getDirectives().end(); ++it) {
        std::cout << std::string(level + 1, '\t') << "(" << it->first << ") : ";
        for (size_t i = 0; i < it->second.size(); ++i) {
            std::cout << it->second[i] << " ";
        }
        std::cout << std::endl;
    }

    for (std::vector<Context*>::const_iterator it = context->getChildren().begin(); it != context->getChildren().end(); ++it) {
        display(*it, level + 1);
    }
}

void CORE::display(const struct addrinfo* addrinfo) {
    std::cout << "ai_flags: " << int(addrinfo->ai_flags) << std::endl;
    std::cout << "ai_family: " << int(addrinfo->ai_family) << std::endl;
    std::cout << "ai_socktype: " << int(addrinfo->ai_socktype) << std::endl;
    std::cout << "ai_protocol: " << int(addrinfo->ai_protocol) << std::endl;
    std::cout << "ai_addrlen: " << int(addrinfo->ai_addrlen) << std::endl;
    std::cout << "ai_canonname: " << (addrinfo->ai_canonname == NULL ? "null" : addrinfo->ai_canonname) << std::endl;
    CORE::display(addrinfo->ai_addr);
    int count = 0;
    for (const struct addrinfo* it = addrinfo->ai_next; it != NULL; it = it->ai_next) {
        ++count;
    }
    std::cout << "ai_next: " << int(count) << std::endl;
}

void CORE::display(const struct sockaddr* addr) {
    std::cout << "ai_addr: " << std::endl;
    if (addr->sa_family == AF_INET) {
        char ipv4[INET_ADDRSTRLEN];
        const struct sockaddr_in* addrIpv4 = reinterpret_cast<const struct sockaddr_in*>(addr);
        std::cout << "\tsin_len: " << int(addrIpv4->sin_len) << std::endl;
        std::cout << "\tsin_family: " << int(addrIpv4->sin_family) << std::endl;
        std::cout << "\tsin_port: " << ntohs(addrIpv4->sin_port) << std::endl;
        std::cout << "\tsin_addr: " << inet_ntop(AF_INET, &addrIpv4->sin_addr, ipv4, INET_ADDRSTRLEN) << std::endl;
        std::cout << "\tsin_zero: [8]" << std::endl;
    } else if (addr->sa_family == AF_INET6) {
        char ipv6[INET6_ADDRSTRLEN];
        const struct sockaddr_in6* addrIpv6 = reinterpret_cast<const struct sockaddr_in6*>(addr);
        std::cout << "\tsin6_len: " << int(addrIpv6->sin6_len) << std::endl;
        std::cout << "\tsin6_family: " << int(addrIpv6->sin6_family) << std::endl;
        std::cout << "\tsin6_port: " << ntohs(addrIpv6->sin6_port) << std::endl;
        std::cout << "\tsin6_flowinfo: " << int(addrIpv6->sin6_flowinfo) << std::endl;
        std::cout << "\tsin6_addr: " << inet_ntop(AF_INET6, &addrIpv6->sin6_addr, ipv6, INET6_ADDRSTRLEN) << std::endl;
        std::cout << "\tsin6_scope_id: " << int(addrIpv6->sin6_scope_id) << std::endl;
    }
}
# endif
// *************************************************************************************************************************************

// * Functions *************************************************************************************************************************
const std::vector<std::string> CORE::split(const std::string& str, const std::string& delimiters) {
    std::vector<std::string> tokens;
    std::string token;
    bool escape = false;

    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '\\') {
            escape = true;
            continue;
        }

        if (delimiters.find(str[i]) != std::string::npos && escape == false) {
            if (token.empty() == false) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += str[i];
        }
        escape = false;
    }

    if (token.empty() == false) {
        tokens.push_back(token);
    }

    return tokens;
}

static void signalHandler(int signal) {
    if (signal == SIGINT) {
        Webserv::webservState = WEB_SERV_STOPPING;
    }
}

void CORE::listenToSignals() {
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signalHandler;
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        throw std::runtime_error("sigaction() : " + std::string(strerror(errno)));
    }
}

Client* CORE::getClientWithFd(const int fd, const std::vector<Client*>& clients) {
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i]->getType() == USER_CLIENT) {
            if (clients[i]->getFd() == fd) {
                return clients[i];
            }
        } else if (clients[i]->getType() == CGI_CLIENT) {
            if (clients[i]->getPipeFd()[READ_END] == fd) {
                return clients[i];
            } else if (clients[i]->getPipeFd()[WRITE_END] == fd) {
                return clients[i];
            }
        }
    }
    return nullptr; // never be reached (i hope so :D)
}
// *************************************************************************************************************************************