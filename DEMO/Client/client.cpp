// Includes
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <unistd.h>
# include <iostream>
# include <string>
# include "../tools/tools.hpp"

int main(int argc, char **argv) {
    int             status;
    int             sockfd;
    struct addrinfo hints, *res, *current;

    // check the number of arguments
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " [ hostname ? _ ] [ (port | service) ? _ ] [ socket type (S | D) ] [ network type (4 | 6) ] " << std::endl;
        exit(EXIT_FAILURE);
    }

    // fill the hostname and port
    char *hostname = (argv[1][0] != '_' ? argv[1] : NULL);
    char *port = (argv[2][0] != '_' ? argv[2] : NULL);

    // make sure the struct is empty
    memset(&hints, 0, sizeof(hints));
    // fill the hints
    hints.ai_family = (argv[4][0] == '4' ? AF_INET : (argv[4][0] == '6' ? AF_INET6 : AF_UNSPEC));
    hints.ai_socktype = (argv[3][0] == 'S' ? SOCK_STREAM : (argv[3][0] == 'D' ? SOCK_DGRAM : 0));

    if ((status = getaddrinfo(hostname, port, &hints, &res)) != 0) {
        std::cerr << "ERROR : getaddrinfo() : " << gai_strerror(status) << std::endl;
        exit(EXIT_FAILURE);
    }

    for (current = res; current != NULL; current = current->ai_next) {
        // create the socket
        if ((sockfd = socket(current->ai_family, current->ai_socktype, current->ai_protocol)) == -1) {
            std::cerr << "ERROR : socket() : " << strerror(errno) << std::endl;
            continue;
        }

        // connect to the server (STREAM_SOCKET)
        if (current->ai_socktype == SOCK_STREAM) {
            if (connect(sockfd, current->ai_addr, current->ai_addrlen) == -1) {
                std::cerr << "ERROR : connect() : " << strerror(errno) << std::endl;
                close(sockfd);
                continue;
            }
        }

        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Connected successfully to : " << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        printAddrInfo(current);
        break;
    }

    if (current == NULL) {
        std::cerr << "ERROR : client failed to connect" << std::endl;
        exit(EXIT_FAILURE);
    }

    while (true) {
        std::string input;
        std::cout << ">> "; 
        std::getline(std::cin, input);
        if (current->ai_socktype == SOCK_STREAM) {
            if (send(sockfd, input.c_str(), input.size(), 0) == -1) {
                std::cerr << "ERROR : send() : " << strerror(errno) << std::endl;
                break; 
            }
        } else if (current->ai_socktype == SOCK_DGRAM) {
            if (sendto(sockfd, input.c_str(), input.size(), 0, current->ai_addr, current->ai_addrlen) == -1) {
                std::cerr << "ERROR : sendto() : " << strerror(errno) << std::endl;
                break;
            }
        }
    }

    // close the socket
    close(sockfd);
    // free the linked list
    freeaddrinfo(res);
}