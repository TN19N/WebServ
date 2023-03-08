# ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <string>
# include <vector>
# include <poll.h>

# include "context.hpp"
# include "client.hpp"

# define WEB_SERV_RUNNING 0
# define WEB_SERV_STOPPING 1

class Webserv {
    private:
        const Context* const configuration;

        std::vector<int>                serversSocketFd;
        std::vector<Client*>            clients;

        void startServers();

        void removeClient(const Client* client);
    public:
        Webserv(const std::string& configFile);

        static bool webservState;

        void run();

        ~Webserv();
};

# endif