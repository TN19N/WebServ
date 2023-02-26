# include "webserv/client.hpp"
# include "webserv/context.hpp"
# include "webserv/http.hpp"

Client* HTTP::runCgi(const Context* location, const Client* client) {
    int     pipeFd[2];
    pid_t   pid;
}