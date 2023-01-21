# ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <vector>

# include "server.hpp"

/**
 * class WebServ - represents the whole web server
 */

class WebServ {
    public :
        static std::vector<Server> servers;
};

# endif