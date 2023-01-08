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
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " [ hostname ? _ ] [ (port | service) ? _ ]" << std::endl;
        exit(EXIT_FAILURE);
    }

    // fill the hostname and port
    char *hostname = (argv[1][0] != '_' ? argv[1] : NULL);
    char *port = (argv[2][0] != '_' ? argv[2] : NULL);

    // make sure the struct is empty
    memset(&hints, 0, sizeof(hints));
    // fill the hints
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(hostname, port, &hints, &res)) != 0) {
        std::cerr << "ERROR : getaddrinfo() : " << gai_strerror(status) << std::endl;
        exit(EXIT_FAILURE);
    }

    # ifdef DEBUG
    std::cerr << "----------------------------------------" << std::endl;
    std::cerr << "DEBUG : " << std::endl;
    std::cerr << "----------------------------------------" << std::endl;
    for (current = res; current != NULL; current = current->ai_next) {
        printAddrInfo(current, std::cerr);
        if (current->ai_next != NULL) {
            std::cerr << "----------------------------------------" << std::endl;
        }
    }
    # endif

    for (current = res; current != NULL; current = current->ai_next) {
        // create the socket
        if ((sockfd = socket(current->ai_family, current->ai_socktype, current->ai_protocol)) == -1) {
            std::cerr << "ERROR : socket() : " << strerror(errno) << std::endl;
            continue;
        }

        // connect to the server
        if (connect(sockfd, current->ai_addr, current->ai_addrlen) == -1) {
            std::cerr << "ERROR : connect() : " << strerror(errno) << std::endl;
            close(sockfd);
            continue;
        }

        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Connected successfully to : " << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        printAddrInfo(current, std::cout);
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
    }

    // close the socket
    close(sockfd);
    // free the linked list
    freeaddrinfo(res);
}