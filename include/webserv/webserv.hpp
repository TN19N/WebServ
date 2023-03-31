# ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <string>
# include <vector>
# include <poll.h>

# include "webserv/defines.hpp"
# include "webserv/context.hpp"
# include "webserv/client.hpp"

class Webserv {
    private:
        const Context* const configuration;

        std::vector<int>                serversSocketFd;
        std::vector<Client*>            clients;

        void startServers();

        void removeClient(const Client* client);
		void errorHandler(int statusCode, Client* client);
		void checkClientsTimeout();
    public:
        Webserv(const std::string& configFile);

        static bool webservState;

        void run();

        ~Webserv();
};

# endif