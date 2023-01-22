# ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <vector>

# include "server.hpp"

class WebServ {
    public :
        static std::vector<Server> servers;
};

# endif