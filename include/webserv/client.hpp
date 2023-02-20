# ifndef CLIENT_HPP
# define CLIENT_HPP

# include <sys/socket.h>
# include <string>

# include "webserv/request.hpp"

class Client {
    private:
        int                     fd;
        struct sockaddr_storage addr;
        std::string             buffer;
        Request*                request;
    public:
        Client(const int fd, const struct sockaddr_storage& addr);

        // getters
        const int&                     getFd() const;
        const struct sockaddr_storage& getAddr() const;
        const std::string&             getBuffer() const;
        const Request*                 getRequest() const;

        void addBuffer(char *buf, int len);

        void newRequest();

        ~Client();
};

# endif