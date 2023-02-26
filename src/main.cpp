# include <iostream>

# include "webserv/webserv.hpp"

# define DEFAULT_CONFIG_FILE_PATH "config/default.cfg"

int main(int argc, const char* const* argv) {
    try {
        if (argc > 2) {
            throw std::runtime_error("usage: webserv [config_file_path]");
        }
        WebServer::getInstance(argc == 2 ? argv[1] : DEFAULT_CONFIG_FILE_PATH).run();
    } catch (const std::exception& e) {
        std::cerr << "webserv: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
