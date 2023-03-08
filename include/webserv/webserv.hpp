# ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <string>
# include <vector>
# include <poll.h>

# include "webserv/context.hpp"
# include "webserv/client.hpp"

# define WEB_SERV_RUNNING 0
# define WEB_SERV_STOPPING 1

class Webserv {
    private:
        const Context* const configuration;

        std::vector<pollfd>             fds;
        std::vector<struct addrinfo*>   serversAddress;

        std::vector<Client*>            clients;

        void startServers();
    public:
        Webserv(const std::string& configFile);

        static bool webservState;

        void run();

        ~Webserv();
};

# endif