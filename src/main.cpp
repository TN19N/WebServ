# include <iostream>

# include "definitions.hpp"
# include "webServ.hpp"

int main(int argc, char** argv) {
    if (argc > 2) {
        std::cerr << "webserv: Usage: webserv [config_file]." << std::endl;
        exit(EXIT_FAILURE);
    }

    Webserv webserv(argc == 2 ? argv[1] : DEFAULT_CONFIG_FILE);

    webserv.run();
}