# include <iostream>
# include <wordexp.h>

# include "webserv/webserv.hpp"

# define DEFAULT_CONFIG_FILE_PATH "./config/config.cfg"

# define RED    "\033[31m"
# define RESET  "\033[0m"

# ifdef DEBUG
static void checkLeaks() {
    std::cout << "-------------------------- Leaks --------------------------" << std::endl;
    system("leaks webserv 2> /dev/null");
}
# endif

int main(const int argc, const char* const* argv) {

    try {
        if (argc > 2) {
            throw std::invalid_argument("usage: webserv [config_file_path].");
        }
        Webserv(argc == 2 ? argv[1] : DEFAULT_CONFIG_FILE_PATH).run();
    } catch (const std::exception& e) {
        std::cerr << "webserv: " << e.what() << " ." << std::endl;

        # ifdef DEBUG
        checkLeaks();
        # endif

        return EXIT_FAILURE;
    }

    # ifdef DEBUG
    checkLeaks();
    # endif

    std::cerr << RED << std::endl;
    std::cerr << "************************************" << std::endl;
    std::cerr << "* webserv: you exited the program. *" << std::endl;
    std::cerr << "************************************" << std::endl;
    std::cerr << RESET;

    return EXIT_SUCCESS;
}