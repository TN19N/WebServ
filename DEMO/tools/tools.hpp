// Includes
# include <arpa/inet.h>
# include <iostream>

// print all information about the addrinfo struct
void printAddrInfo(const struct addrinfo *current) {
    // type of the socket
    std::cout << "ai_socktype: ";
    if (current->ai_socktype == SOCK_STREAM) {
        std::cout << "SOCK_STREAM" << std::endl;
    } else if (current->ai_socktype == SOCK_DGRAM) {
        std::cout << "SOCK_DGRAM" << std::endl;
    }
    // type of the network (ipv4 - ipv6)
    std::cout << "ai_family: ";
    if (current->ai_family == AF_INET) {
        std::cout << "AF_INET" << std::endl;
    } else if (current->ai_family == AF_INET6) {
        std::cout << "AF_INET6" << std::endl;
    }
    // type of the protocol (TCP - UDP)
    std::cout << "ai_protocol: ";
    if (current->ai_protocol == IPPROTO_TCP) {
        std::cout << "IPPROTO_TCP" << std::endl;
    } else if (current->ai_protocol == IPPROTO_UDP) {
        std::cout << "IPPROTO_UDP" << std::endl;
    }
    // addr struct length
    std::cout << "ai_addrlen: " << current->ai_addrlen << std::endl;
    // the canonical name of the specified hostname
    std::cout << "ai_canonname: ";
    if (current->ai_canonname != NULL) {
        std::cout << current->ai_canonname << std::endl;
    } else {
        std::cout << "NULL" << std::endl;
    }
    // ai_addr struct infos
    in_port_t port;
    char      ipstr[INET6_ADDRSTRLEN];
    std::cout << "ai_addr: ";
    if (current->ai_family == AF_INET) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *) current->ai_addr;
        inet_ntop(AF_INET, &(ipv4->sin_addr), ipstr, INET_ADDRSTRLEN);
        port = ntohs(ipv4->sin_port);
    } else if (current->ai_family == AF_INET6) {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) current->ai_addr;
        inet_ntop(AF_INET6, &(ipv6->sin6_addr), ipstr, INET6_ADDRSTRLEN);
        port = ntohs(ipv6->sin6_port);
    }
    std::cout << "(Port : " << port << ") (Ip : " << ipstr << ")" << std::endl;
}