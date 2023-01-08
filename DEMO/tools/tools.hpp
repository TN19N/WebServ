// Includes
# include <arpa/inet.h>
# include <iostream>

// print all information about the addrinfo struct
void printAddrInfo(const struct addrinfo *current, std::ostream &out) {
    out << "ai_socktype: ";
    if (current->ai_socktype == SOCK_STREAM) {
        out << "SOCK_STREAM" << std::endl;
    } else if (current->ai_socktype == SOCK_DGRAM) {
        out << "SOCK_DGRAM" << std::endl;
    } else {
        out << " ? " << std::endl;
    }

    out << "ai_family: ";
    if (current->ai_family == AF_INET) {
        out << "AF_INET" << std::endl;
    } else if (current->ai_family == AF_INET6) {
        out << "AF_INET6" << std::endl;
    } else {
        out << " ? " << std::endl;
    }

    out << "ai_protocol: ";
    if (current->ai_protocol == IPPROTO_TCP) {
        out << "IPPROTO_TCP" << std::endl;
    } else if (current->ai_protocol == IPPROTO_UDP) {
        out << "IPPROTO_UDP" << std::endl;
    } else {
        out << " ? " << std::endl;
    }

    out << "ai_addrlen: " << current->ai_addrlen << std::endl;

    out << "ai_canonname: ";
    if (current->ai_canonname != NULL) {
        out << current->ai_canonname << std::endl;
    } else {
        out << "NULL" << std::endl;
    }

    in_port_t port;
    char      ipstr[INET6_ADDRSTRLEN];
    out << "ai_addr: ";
    if (current->ai_family == AF_INET) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *) current->ai_addr;
        inet_ntop(AF_INET, &(ipv4->sin_addr), ipstr, INET_ADDRSTRLEN);
        port = ntohs(ipv4->sin_port);
    } else if (current->ai_family == AF_INET6) {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) current->ai_addr;
        inet_ntop(AF_INET6, &(ipv6->sin6_addr), ipstr, INET6_ADDRSTRLEN);
        port = ntohs(ipv6->sin6_port);
    } else {
        port = -1;
        ipstr[0] = '?';
        ipstr[1] = '\0';
    }
    out << "(Port : " << port << ") (Ip : " << ipstr << ")" << std::endl;
}