// Includes
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <unistd.h>
# include <iostream>
# include "../tools/tools.hpp"

# define BACKLOG 10

int main(int argc, char **argv) {
    int             status;
    int             sockfd;
    struct addrinfo hints, *res, *current;

    // check the number of arguments
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " < port | service >" << std::endl;
        exit(EXIT_FAILURE);
    }

    // fill the port
    char *port = argv[1];

    // make sure the struct is empty
    memset(&hints, 0, sizeof(hints));
    // fill the hints
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    // hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, port, &hints, &res)) != 0) {
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

        // bind the socket to the port
        if (bind(sockfd, current->ai_addr, current->ai_addrlen) == -1) {
            std::cerr << "ERROR : bind() : " << strerror(errno) << std::endl;
            close(sockfd);
            continue;
        }

        // listen for connections
        if (listen(sockfd, BACKLOG) == -1) {
            std::cerr << "ERROR : listen() : " << strerror(errno) << std::endl;
            close(sockfd);
            continue;
        }

        std::cout << "----------------------------------------" << std::endl;
        std::cout << "server created successfully : " << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        printAddrInfo(current, std::cout);
        break;
    }

    if (current == NULL) {
        std::cerr << "ERROR : server failed to create" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Server is listening for connections ..." << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    while (true) {
        struct sockaddr_storage client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int       client_fd;
        if ((client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_size)) == -1) {
            std::cerr << "accept() : " << strerror(errno) << std::endl;
            continue;
        }

        std::cerr << "Connect to client [ID : " << client_fd << "]" << std::endl;        
    }

    // close the socket
    close(sockfd);
    // free the linked list
    freeaddrinfo(res);
}