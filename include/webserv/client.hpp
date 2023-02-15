# ifndef CLIENT_HPP
# define CLIENT_HPP

# include <sys/socket.h>
# include <string>

class Client {
    private:
        int                     fd;
        struct sockaddr_storage addr;
        std::string             buffer;
    public:
        Client(const int fd, const struct sockaddr_storage& addr);

        const int& getFd() const;
};

# endif