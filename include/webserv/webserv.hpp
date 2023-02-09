# ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <string>

# include "webserv/context.hpp"

class WebServer {
    public :
        static const WebServer& getInstance(const std::string& configFilePath);
        void run() const;

    private:
        static const WebServer* instance;
        WebServer(const std::string& configFilePath);
        ~WebServer();

        const Context* const configuration;
};

# endif