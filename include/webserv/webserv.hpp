# ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <string>
# include <vector>
# include <poll.h>

# include "defines.hpp"
# include "context.hpp"
# include "client.hpp"

class Webserv {
    private:
        const Context* const configuration;

        std::vector<int>                serversSocketFd;
        std::vector<Client*>            clients;

        void startServers();

        void removeClient(const Client* client);
		void errorHandler(int statusCode, Client* client);
    public:
        Webserv(const std::string& configFile);

        static bool webservState;

        void run();

        ~Webserv();
};

# endif