# include <iostream>

# include "definitions.hpp"

# include "parser.hpp"

int main(int argc, char** argv) {
    if (argc > 2) {
        std::cerr << "webserv: usage: webserv [config_file]" << std::endl;
        exit(EXIT_FAILURE);
    }

    parseConfigFile(argc == 2 ? argv[1] : DEFAULT_CONFIG_FILE);
}