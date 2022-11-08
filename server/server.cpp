#include "../includes/tools.hpp"
#include "./server.hpp"

int main()
{
    try {
        Server server(8080, SOCK_STREAM, AF_INET);
        server.startListening();
    } catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}