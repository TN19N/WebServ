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
        const Client*           cgiClient;
    public:
        Client(int, const struct sockaddr_storage&, const struct sockaddr_storage&, const Client* = nullptr);

        const int&                     getFd() const;
        const struct sockaddr_storage& getAddr() const;
        const struct sockaddr_storage& getPeer() const;
        const std::string&             getBuffer() const;
        Request*                 getRequest() const;

        void    newRequest(Request*);
        void    addBuffer(char *, int);
		void	setBuffer(const char *) ;

        ~Client();
};

# endif