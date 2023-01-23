# include <vector>
# include <poll.h>
# include <iostream>
# include <unistd.h>
# include <string>

# include "server.hpp"
# include "webServ.hpp"
# include "init.hpp"

// init static members
std::vector<Server>         WebServ::servers;
std::vector<struct pollfd>  WebServ::pollFds;

static void panic(const std::string& msg) {
    std::cerr << "webserv: running error : " << msg << std::endl;
    WebServ::clear();
    exit(EXIT_FAILURE);
}

void WebServ::clear() {
    WebServ::servers.clear();
    
    for (size_t i = 0; i < WebServ::pollFds.size(); ++i) {
        close(WebServ::pollFds[i].fd);
    }
    WebServ::pollFds.clear();
}

void WebServ::run() {
    init();

    int num_events = 0;
    while (true) {
        if ((num_events = poll(WebServ::pollFds.data(), WebServ::pollFds.size(), -1)) == -1) {
            panic(std::string("poll() : ") + strerror(errno));
        }

        for (size_t i = 0; i < WebServ::pollFds.size() && num_events != 0; ++i) {
            if (WebServ::pollFds[i].revents & POLLIN) {
                --num_events;
            }
        }
    }
}