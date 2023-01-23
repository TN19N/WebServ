# ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <vector>
# include <poll.h>

# include "server.hpp"

class WebServ {
    public :
        static std::vector<Server> servers;
        static std::vector<struct pollfd> pollFds;
    public:
        static void run();
        static void clear();
};

# endif