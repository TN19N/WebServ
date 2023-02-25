# ifndef CLIENT_HPP
# define CLIENT_HPP

# include <sys/socket.h>
# include <string>

# include "webserv/request.hpp"

class Client {
    private:
        int                     fd;
        struct sockaddr_storage addr;
        struct sockaddr_storage peer;
        std::string             buffer;
        Request*                request;
    public:
        Client(const int fd, const struct sockaddr_storage& addr, const struct sockaddr_storage& peer);

        const int&                     getFd() const;
        const struct sockaddr_storage& getAddr() const;
        const struct sockaddr_storage& getPeer() const;
        const std::string&             getBuffer() const;
        const Request*                 getRequest() const;

        void    newRequest(Request* request);
        void    addBuffer(char *buf, int len);

        ~Client();
};

# endif