// Includes
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <unistd.h>
# include <iostream>
# include <poll.h>
# include <vector>
# include "../tools/tools.hpp"

# define BACKLOG    10
# define MAXBUFSIZE 1000

int main(int argc, char **argv) {
    int             status;
    int             sockfd;
    struct addrinfo hints, *res, *current;

    // check the number of arguments
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " < port | service > < socket type (S | D) > < network type (4 | 6) > " << std::endl;
        exit(EXIT_FAILURE);
    }

    // fill the port
    char *port = argv[1];

    // make sure the struct is empty
    memset(&hints, 0, sizeof(hints));
    // fill the hints
    hints.ai_family = (argv[3][0] == '4' ? AF_INET : (argv[3][0] == '6' ? AF_INET6 : AF_UNSPEC));
    hints.ai_socktype = (argv[2][0] == 'S' ? SOCK_STREAM : (argv[2][0] == 'D' ? SOCK_DGRAM : 0));

    if ((status = getaddrinfo(NULL, port, &hints, &res)) != 0) {
        std::cerr << "ERROR : getaddrinfo() : " << gai_strerror(status) << std::endl;
        exit(EXIT_FAILURE);
    }

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

        std::cout << "----------------------------------------" << std::endl;
        std::cout << "server created successfully : " << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        printAddrInfo(current);
        break;
    }

    if (current == NULL) {
        std::cerr << "ERROR : server failed to create" << std::endl;
        exit(EXIT_FAILURE);
    }

    // listen for connections (SOCK_STREAM only)
    if (current->ai_socktype == SOCK_STREAM) {
        if (listen(sockfd, BACKLOG) == -1) {
            std::cerr << "ERROR : listen() : " << strerror(errno) << std::endl;
            close(sockfd);
            exit(EXIT_FAILURE);
        }
    }

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Server is listening for connections ..." << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    char buffer[MAXBUFSIZE];
    if (current->ai_socktype == SOCK_DGRAM) {
        while (true) {
            struct sockaddr_storage client_addr;
            socklen_t client_addr_len = sizeof(client_addr);
            int bytes_received = recvfrom(sockfd, buffer, MAXBUFSIZE - 1, 0, (struct sockaddr *)&client_addr, &client_addr_len);
            if (bytes_received == -1) {
                std::cerr << "ERROR : recvfrom() : " << strerror(errno) << std::endl;
                close(sockfd);
                exit(EXIT_FAILURE);
            }
            buffer[bytes_received] = '\0';
            std::cout << "anonymous client : " << buffer << std::endl;
        }
    } else if (current->ai_socktype == SOCK_STREAM) {
        std::vector<struct pollfd> fds;
        fds.push_back({sockfd, POLLIN, 0});
        while (true) {
            int num_ready = poll(fds.data(), fds.size(), -1);
            if (num_ready == -1) {
                std::cerr << "ERROR : poll() : " << strerror(errno) << std::endl;
                close(sockfd);
                exit(EXIT_FAILURE);
            }
            for (int i = 0; i < fds.size(); ++i) {
                if (fds[i].revents & POLLIN) {
                    if (fds[i].fd == sockfd) {
                        struct sockaddr_storage client_addr;
                        socklen_t client_addr_len = sizeof(client_addr);
                        int new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
                        if (new_fd == -1) {
                            std::cerr << "ERROR : accept() : " << strerror(errno) << std::endl;
                            close(sockfd);
                            exit(EXIT_FAILURE);
                        }
                        fds.push_back({new_fd, POLLIN, 0});
                        std::cout << "new client connected under ID : " << new_fd - 3 << std::endl;
                    } else {
                        int bytes_received = recv(fds[i].fd, buffer, MAXBUFSIZE - 1, 0);
                        if (bytes_received <= 0) {
                            if (bytes_received == 0) {
                                std::cout << "client " << fds[i].fd - 3 << " disconnected" << std::endl;
                            }
                            if (bytes_received == -1) {
                                std::cerr << "ERROR : recv() : " << strerror(errno) << std::endl;
                            }
                            close(fds[i].fd);
                            fds.erase(fds.begin() + i);
                        } else {
                            buffer[bytes_received] = '\0';
                            std::cout << "client [ID : " << fds[i].fd - 3 << "] : " << buffer << std::endl;
                        }
                    }
                }
            }
        }
    }

    // close the socket
    close(sockfd);
    // free the linked list
    freeaddrinfo(res);
}