# ifndef SERVER_HPP
# define SERVER_HPP

# include <arpa/inet.h>
# include <string>
# include <vector>

# include "location.hpp"

class Server {
    private:
        std::string             host;
        in_port_t               port;
        std::string             name;
        std::vector<Location>   locations;
    public:
        // constructor
        Server(const std::vector<std::string>& contextArgs);

        // getters
        const std::string&              getHost()       const;
        const in_port_t&                getPort()       const;
        const std::string&              getName()       const;
        const std::vector<Location>&    getLocations()  const;
        std::vector<Location>&          getLocations();

        // setters
        void setHost(const std::vector<std::string>& directiveArgs);
        void setPort(const std::vector<std::string>& directiveArgs);
        void setName(const std::vector<std::string>& directiveArgs);
        void addLocation(const std::vector<std::string>& directiveArgs);

        // destructor
        ~Server();
};

# endif