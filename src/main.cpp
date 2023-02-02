# include <iostream>

# include "parser.hpp"
# include "context.hpp"
# include "definitions.hpp"

int main(int argc, char** argv) {
    if (argc > 2) {
        std::cerr << "webserv: Usage: webserv [config_file]." << std::endl;
        exit(EXIT_FAILURE);
    }

    Context* mainContext = new Context();
    parseConfigFile(argc == 2 ? argv[1] : DEFAULT_CONFIG_FILE, mainContext);

    
}