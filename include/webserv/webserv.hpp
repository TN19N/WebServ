# ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <string>
# include <vector>
# include <poll.h>

# include "webserv/context.hpp"

class WebServer {
    public :
        static WebServer& getInstance(const std::string& configFilePath);
        void run();

    private:
        static WebServer* instance;
        WebServer(const std::string& configFilePath);
        ~WebServer();

        const Context* const        configuration;
        std::vector<struct pollfd>  fds;
};

# endif