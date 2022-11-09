#include "../includes/tools.hpp"
#include "./client.hpp"

int main(int argc, char **argv)
{
    if (argc == 4)
    {
        try {
            Client client(argv[1], atoi(argv[2]), SOCK_STREAM, AF_INET6, argv[3]);
            client.startConnecting();
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
    else
        std::cout << "Usage: './client [Host] [Port] [Name]' " << std::endl;
}